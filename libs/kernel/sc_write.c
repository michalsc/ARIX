#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

ssize_t SC_write(int fd, const void *buff, size_t size)
{
    return sc3(SYS_write, fd, (long)buff, size);
}

