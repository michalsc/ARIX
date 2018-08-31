#define _GNU_SOURCE
#include <sys/syscall.h>
#include <exec/libraries.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <proto/exec.h>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct Message msg = { NULL, NULL, 0, 0, 100 };
    struct MsgPort arix = {
        MAKE_UUID(0x00000001, 0x0000, 0x4000, 0x8000 | NT_MSGPORT, 0x000000000000),
        0, NULL
    };
    struct MsgPort *reply = CreateMsgPort();

    msg.mn_ReplyPort = reply;
    int i;
    printf("doing 100000 iterations\n");
    for (i=0; i < 10; i++) {
        PutMsg(&arix, &msg);
        WaitPort(reply);
        DiscardMsg(GetMsg(reply));
    }

    // Use c lib's printf. We are allowed to do that.
    printf("Testing memory allocations\n");
    printf("SysBase = %p\n", ExecBase);
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
}
