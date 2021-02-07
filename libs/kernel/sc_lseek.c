#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

off_t SC_lseek(int fd, off_t offset, int whence)
{
    return sc3(SYS_lseek, fd, offset, whence);
}
