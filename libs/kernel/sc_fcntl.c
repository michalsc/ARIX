#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_fcntl(int fd, int cmd, long arg)
{
    return sc3(SYS_fcntl, fd, cmd, arg);
}
