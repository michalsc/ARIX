#include <exec/libraries.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <proto/exec.h>

#include <stdio.h>

int main(int argc, char **argv)
{
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
