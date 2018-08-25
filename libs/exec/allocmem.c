#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <clib/exec_protos.h>

void * AllocMem(size_t byteSize, uint32_t requirements)
{
    void *ptr = malloc(byteSize);

    if (ptr) {
        if (requirements & MEMF_CLEAR) {
            bzero(ptr, byteSize);
        }
    }

    return ptr;
}
