#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_open(const char *pathname, int flags)
{
    return sc3(SYS_openat, AT_FDCWD, (long)pathname, (int)flags);
}

int SC_openat(int dirfd, const char *pathname, int flags)
{
    return sc3(SYS_openat, dirfd, (long)pathname, (int)flags);
}

int SC_creat(const char *pathname, mode_t mode)
{
    return sc4(SYS_openat, AT_FDCWD, (long)pathname, O_CREAT|O_WRONLY|O_TRUNC, (long)mode);
}
