#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

void SC_exit(int status)
{
    sc1nr(SYS_exit, status);
}

