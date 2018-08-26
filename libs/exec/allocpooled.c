#include <exec/types.h>
#include <exec/memory.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void *AllocPooled(void * p, size_t byteSize)
{
    struct PrivPool *pool = p;
    void *ret = NULL;

    if (pool)
    {
        ret = tlsf_malloc(pool->handle, byteSize, &pool->requirements);
    }

    return ret;
}

void *AllocVecPooled(void *p, size_t byteSize)
{
    return AllocPooled(p, byteSize);
}