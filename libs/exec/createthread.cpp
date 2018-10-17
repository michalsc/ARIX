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
#include <sys/types.h>
#include <sys/un.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>
#include <utility/tagitem.h>
#include <exec/tasks.h>
#include <exec/memory.h>

#include "exec_intern.h"

int __thread_bootstrap(void *arg);

void CreateThread(struct TagItem *tags)
{
    struct TagItem *tempTag = static_cast<struct TagItem *>(AllocVec(2*sizeof(struct TagItem), 0));
    IPTR stack_size = LibGetTagData(TASKTAG_STACKSIZE, 40960, tags);
    void *stack = NULL;
    
    stack_size = (stack_size + 4095) & ~4095;
    stack = AllocVecAligned(stack_size, 32, MEMF_CLEAR);

    bug("[EXEC] CreateThread(%p)\n", tags);
    bug("[EXEC] Allocated %d KB stack at %p\n", stack_size >> 10, stack);

    tempTag[0].ti_Tag = TASKTAG_AFFINITY + 1;
    tempTag[0].ti_Data = reinterpret_cast<IPTR>(stack);
    tempTag[1].ti_Tag = TAG_MORE;
    tempTag[1].ti_Data = reinterpret_cast<IPTR>(tags);

    stack = (UBYTE *)stack + stack_size;

    bug("[EXEC] Calling clone()\n");

    int ret = clone(__thread_bootstrap, stack, 
        CLONE_VM | CLONE_THREAD | CLONE_SIGHAND | CLONE_FS | CLONE_FILES | CLONE_IO, // CLONE_PARENT,
        tempTag);
}

int __thread_bootstrap(void *arg)
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
        syscall(SYS_prctl, PR_SET_NAME, t->ti_Data);
    }

    bug("[EXEC] CreateThread(): inside bootstrap\n");
    bug("[EXEC] pid=%d, tid=%d\n", syscall(SYS_getpid), syscall(SYS_gettid));

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

    FreeVec((void*)tags[0].ti_Data);
    FreeVec(tags);
}