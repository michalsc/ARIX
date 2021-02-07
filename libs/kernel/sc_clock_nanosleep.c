#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_clock_nanosleep(clockid_t clockid, int flags, const struct timespec *request, struct timespec *remain)
{
    return sc4(SYS_clock_gettime, clockid, flags, (long)request, (long)remain);
}

