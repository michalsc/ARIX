#include <exec/types.h>
#include <exec/memory.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void DeletePool(void * p)
{
    struct PrivPool *pool = p;

    if (pool)
    {
        tlsf_destroy(pool->handle);
        FreeMem(pool, sizeof(struct PrivPool));
    }
}