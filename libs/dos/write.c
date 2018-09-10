#include <unistd.h>
#include <clib/dos_protos.h>

size_t Write(uuid_t handle, const void *buffer, size_t length)
{
    (void)handle;
    (void)buffer;
    (void)length;
    return 0;
}