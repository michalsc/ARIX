#include <exec/types.h>
#include <exec/memory.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void *ReallocPooled(void * p, void * memoryBLock, size_t newSize)
{
    struct PrivPool *pool = p;
    
    void *ret = NULL;

    if (pool)
    {
        ret = tlsf_realloc(pool->handle, memoryBLock, newSize);
    }

    return ret;
}
