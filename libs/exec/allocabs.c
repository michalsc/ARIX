#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void *AllocAbs(size_t byteSize, void * location)
{
    void *ptr = tlsf_allocabs(local_memory_pool, byteSize, location);

    return ptr;
}
