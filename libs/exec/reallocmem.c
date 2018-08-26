#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void *ReallocMem(void * memoryBlock, size_t newSize)
{
    void *ptr = tlsf_realloc(local_memory_pool, memoryBlock, newSize);

    return ptr;
}
