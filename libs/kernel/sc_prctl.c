#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_prctl(int option, unsigned long arg2, unsigned long arg3,
                 unsigned long arg4, unsigned long arg5)
{
    return sc5(SYS_prctl, option, arg2, arg3, arg4, arg5);
}
