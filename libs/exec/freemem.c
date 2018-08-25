#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <clib/exec_protos.h>

void FreeMem(void * ptr, size_t byteSize)
{
    (void)byteSize;
    free(ptr);
}