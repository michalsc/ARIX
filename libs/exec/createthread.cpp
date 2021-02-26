/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <stdlib.h>
#include <utility/hooks.h>
#include <clib/exec_protos.h>
#include <proto/kernel.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>
#include <utility/tagitem.h>
#include <exec/tasks.h>
#include <exec/memory.h>

#define DEBUG

#include "exec_intern.h"
#include "exec_debug.h"

void __attribute__((noreturn)) __thread_bootstrap(void *arg);

void CreateThread(struct TagItem *tags)
{
    struct TagItem *tempTag = static_cast<struct TagItem *>(AllocVec(2*sizeof(struct TagItem), 0));
    IPTR stack_size = LibGetTagData(TASKTAG_STACKSIZE, 40960, tags);
    void *stack = NULL;

    stack_size = (stack_size + 4095) & ~4095;
    stack = AllocVecAligned(stack_size, 32, MEMF_CLEAR);

    D(bug("[EXEC] CreateThread(%p)\n", tags));
    D(bug("[EXEC] Allocated %d KB stack at %p\n", stack_size >> 10, stack));

    tempTag[0].ti_Tag = TASKTAG_AFFINITY + 1;
    tempTag[0].ti_Data = reinterpret_cast<IPTR>(stack);
    tempTag[1].ti_Tag = TAG_MORE;
    tempTag[1].ti_Data = reinterpret_cast<IPTR>(tags);

    stack = (UBYTE *)stack + stack_size;

    D(bug("[EXEC] Calling clone()\n"));

    /* From this point on lock globally as long as the TagList is in use */
    ObtainMutex(&thread_sync_lock);

    int ret;
    if ((ret = SC_clone(CLONE_VM | CLONE_THREAD | CLONE_SIGHAND | CLONE_FS | CLONE_FILES | CLONE_IO,
        stack, NULL, 0, NULL)) == 0) {
        __thread_bootstrap(tempTag);
    }
}

#define TEMP_SMP_SIZE 1024
ULONG __attribute__((aligned(64))) temp_smp[TEMP_SMP_SIZE];

void __attribute__((noreturn)) __thread_bootstrap(void *arg)
{
    struct TagItem *tags = static_cast<struct TagItem *>(arg);
    struct TagItem *t = NULL;
    IPTR func = LibGetTagData(TASKTAG_PC, 0, tags);
    IPTR args[8];
    int highest_arg = 0;

    for (int i=0; i < 8; i++)
    {
        t = LibFindTagItem(TASKTAG_ARG1 + i, tags);
        if (t != NULL)
        {
            args[i] = t->ti_Data;
            highest_arg = i+1;
        }
    }

    t = LibFindTagItem(TASKTAG_NAME, tags);
    if (t != NULL)
    {
        SC_prctl(PR_SET_NAME, t->ti_Data, 0, 0, 0);
    }

    /* Done with TagList, release thread synchronization lock */
    ReleaseMutex(&thread_sync_lock);

    D(bug("[EXEC] CreateThread(): inside bootstrap\n"));
    D(bug("[EXEC] pid=%d, tid=%d\n", SC_getpid(), SC_gettid()));

    switch(highest_arg)
    {
        case 0:
            ((void (*)())func)();
            break;
        case 1:
            ((void (*)(IPTR))func)(args[0]);
            break;
        case 2:
            ((void (*)(IPTR, IPTR))func)(args[0], args[1]);
            break;
        case 3:
            ((void (*)(IPTR, IPTR, IPTR))func)(args[0], args[1], args[2]);
            break;
        case 4:
            ((void (*)(IPTR, IPTR, IPTR, IPTR))func)(args[0], args[1], args[2], args[3]);
            break;
        case 5:
            ((void (*)(IPTR, IPTR, IPTR, IPTR, IPTR))func)(args[0], args[1], args[2], args[3], args[4]);
            break;
        case 6:
            ((void (*)(IPTR, IPTR, IPTR, IPTR, IPTR, IPTR))func)(args[0], args[1], args[2], args[3], args[4], args[5]);
            break;
        case 7:
            ((void (*)(IPTR, IPTR, IPTR, IPTR, IPTR, IPTR, IPTR))func)(args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
            break;
        case 8:
            ((void (*)(IPTR, IPTR, IPTR, IPTR, IPTR, IPTR, IPTR, IPTR))func)(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
            break;
    }

    D(bug("[EXEC] Finishing thread\n"));

    APTR ssp = (APTR)tags[0].ti_Data;

    FreeVec(tags);

    /* Obtain thread sync mutex in order to perform a stack switch to a common thread exit stack */
    ObtainMutex(&thread_sync_lock);

#ifdef __aarch64__

    extern struct Library *KernelBase;

    asm volatile(
        "   mov sp, %[stack] \n"    // Switch to temporary stack
        "   mov x0, %[ssp] \n"      // Free the stack which was assigned to the thread
        "   bl FreeVec \n"
        "   mov x0, %[lock] \n"     // From now on it is not allowed to use stack!
        "   mov x1, %[kernelBase] \n "
        "   bl internalReleaseMutex \n"
        "   ldr x1, [%[lvo], #%[exit]] \n" 
        "   mov x0, xzr \n"
        "   br x1 \n"
    :
    :   [lock]"r"(&thread_sync_lock.m_Lock),
        [ssp]"r"(ssp),
        [kernelBase]"r"(KernelBase),
        [lvo]"r"(KernelBase->lib_LVOTable),
        [exit]"i"(offsetof(struct KernelBaseLVO, SC_exit)),
        [stack]"r"(&temp_smp[TEMP_SMP_SIZE-16])
    :"x0","x1","x2","x3","x4","x5","x6","x7",
     "x9","x10","x11","x12","x13","x14","x15","x16","x8");

#else

    long __attribute__((noreturn)) (*sc)(long, ...) = (long (*)(long, ...))&syscall;

    
    /*
        This is wrong. First is the stack cleaned up, then the thread exits eventually fetching return
        address from the (already deallocated) stack. Hopefully fixed with __builtin_return_address...

        The proper approach will be to provide asm stubs issuing clone() syscall directly, subsequently
        cleaning up the stack and releasing memory.
    */
    FreeVec(ssp);

    /* Get out of here... */
    SC_exit(0);
#endif
    while(1);
}
