#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_clock_settime(clockid_t clockid, const struct timespec *tp)
{
    return sc2(SYS_clock_settime, clockid, (long)tp);
}

