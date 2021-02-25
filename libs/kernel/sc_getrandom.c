#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

ssize_t SC_getrandom(void *buf, size_t buflen, unsigned int flags)
{
    return sc3(SYS_getrandom, (long)buf, buflen, flags);
}
