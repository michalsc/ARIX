#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>

void FreeMem(void * ptr)
{
    free(ptr);
}