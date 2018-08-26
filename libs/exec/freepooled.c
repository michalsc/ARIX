#include <exec/types.h>
#include <exec/memory.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void FreePooled(void *p, void *memoryBlock, size_t byteSize)
{
    struct PrivPool *pool = p;

    if (pool)
    {
        tlsf_freemem(pool->handle, memoryBlock, byteSize);
    }
}

void FreeVecPooled(void *p, void *memoryBlock)
{
    struct PrivPool *pool = p;

    if (pool)
    {
        tlsf_freevec(pool->handle, memoryBlock);
    }
}