#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_clone(unsigned long flags, void *stack, int *parent_tid, unsigned long tls, int *child_tid)
{
    return sc5(SYS_clone, flags, (long)stack, (long)parent_tid, tls, (long)child_tid);
}
