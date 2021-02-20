#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz)
{
    return sc4(SYS_readlinkat, dirfd, (long)pathname, (long)buf, (long)bufsiz);
}
