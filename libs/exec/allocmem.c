#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void * AllocMem(size_t byteSize, uint32_t requirements)
{
    void *ptr = tlsf_malloc(local_memory_pool, byteSize, &requirements);

    return ptr;
}
