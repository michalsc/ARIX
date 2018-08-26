#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void *AllocVecAligned(size_t byteSize, size_t align, uint32_t requirements)
{
    void *ptr = tlsf_malloc_aligned(local_memory_pool, byteSize, align, &requirements);

    return ptr;
}
