#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_clock_gettime(clockid_t clockid, struct timespec *tp)
{
    return sc2(SYS_clock_gettime, clockid, (long)tp);
}

