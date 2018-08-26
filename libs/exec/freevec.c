#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void FreeVec(void *ptr)
{
    tlsf_freevec(local_memory_pool, ptr);
}
