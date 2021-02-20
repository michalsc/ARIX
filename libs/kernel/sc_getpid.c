#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

pid_t SC_getpid()
{
    return (pid_t)sc0(SYS_getpid);
}
