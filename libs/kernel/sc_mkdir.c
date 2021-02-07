#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_mkdir(const char *pathname, mode_t mode)
{
    return sc3(SYS_mkdirat, AT_FDCWD, (long)pathname, mode);
}

int SC_mkdirat(int dirfd, const char *pathname, mode_t mode)
{
    return sc3(SYS_openat, dirfd, (long)pathname, mode);
}