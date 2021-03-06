/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <exec/libraries.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

static const char __attribute__((used)) version[] = "\0$VER: AllocTest 60.0 " VERSION_STRING_DATE;

#define NUM_ITER    1000000

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    struct timespec t0, t1;
    struct ID empty = NULL_ID;
    struct Message msg = { { NULL, NULL, 0, 0, NULL }, NULL, NULL, 0, empty, 0, 0, 0 };
    struct ID arixPort = ARIX_PORT_ID;
    struct MsgPort *reply = CreateMsgPort();
    
    msg.mn_ReplyPort = reply->mp_ID;
    int i;

    AssignPath("FONTS", "SYS:Fonts");

    AddPort(reply, "Some port name");
    FindPort("Some port name");

    printf("doing %d iterations\n", NUM_ITER);
    clock_gettime(CLOCK_REALTIME, &t0);
    for (i=0; i < NUM_ITER; i++) {
        PutMsg(arixPort, &msg);
        WaitPort(reply);
        DiscardMsg(GetMsg(reply));
    }
    clock_gettime(CLOCK_REALTIME, &t1);
    {
        int64_t delta_ns = t1.tv_nsec - t0.tv_nsec;
        int64_t delta_s = t1.tv_sec - t0.tv_sec;
        int64_t delta = delta_s * 1000000000 + delta_ns;
        printf("Elapsed time: %d µs\nTime per iteration: %d ns\n", (uint32_t)(delta / 1000), (uint32_t)(delta / NUM_ITER));
    }

    printf("doing 100000 iterations without reply port\n");
    msg.mn_ReplyPort = empty;
    clock_gettime(CLOCK_REALTIME, &t0);
    for (i=0; i < NUM_ITER; i++) {
        PutMsg(arixPort, &msg);
    }
    clock_gettime(CLOCK_REALTIME, &t1);
    {
        int64_t delta_ns = t1.tv_nsec - t0.tv_nsec;
        int64_t delta_s = t1.tv_sec - t0.tv_sec;
        int64_t delta = delta_s * 1000000000 + delta_ns;
        printf("Elapsed time: %d µs\nTime per iteration: %d ns\n", (uint32_t)(delta / 1000), (uint32_t)(delta / NUM_ITER));
    }

    // Use c lib's printf. We are allowed to do that.
    printf("Testing memory allocations\n");
    printf("SysBase = %p\n", (void *)ExecBase);
    printf("SysBase Version = %d.%d (%s)\n", ExecBase->lib_Version, ExecBase->lib_Revision, ExecBase->lib_IdString);

    void *buffer = AllocVec(100, MEMF_CLEAR);
    printf("AllocMem returned %p\n", buffer);

    void *buffer3 = AllocVec(100, MEMF_CLEAR);
    printf("AllocMem returned %p\n", buffer3);

    void *buffer2 = AllocVecAligned(240000, 262144, MEMF_CLEAR);
    printf("AllocMemAligned returned %p\n", buffer2);

    FreeVec(buffer);
    FreeVec(buffer2);
    FreeVec(buffer3);

    printf("Creating memory pool\n");
    void * pool = CreatePool(MEMF_CLEAR, 16384, 4096);
    printf("Pool pointer: %p\n", pool);
    buffer = AllocPooled(pool, 200);
    printf("Buffer=%p\n", buffer);
    buffer2 = AllocPooled(pool, 50000);
    printf("Buffer2=%p\n", buffer2);
    buffer3 = AllocPooled(pool, 100);
    printf("Buffer3=%p\n", buffer3);

    printf("Deleting pool\n");
    DeletePool(pool);

    RemPort(reply);
}
