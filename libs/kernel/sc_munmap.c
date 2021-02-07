#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_munmap(void *addr, size_t length)
{
    return sc2(SYS_munmap, (long)addr, length);
}
