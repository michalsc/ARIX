#include <clib/exec_protos.h>
#include <string.h>

void CopyMem(const void * source, void * dest, size_t byteSize)
{
    memmove(dest, source, byteSize);
}

void CopyMemQuick(const void * source, void * dest, size_t byteSize)
{
    memmove(dest, source, byteSize);
}