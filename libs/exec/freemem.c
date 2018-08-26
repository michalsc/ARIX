#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void FreeMem(void * ptr, size_t byteSize)
{
    tlsf_freemem(local_memory_pool, ptr, byteSize);
}