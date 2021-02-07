#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_memfd_create(const char *name, unsigned int flags)
{
    return sc2(SYS_memfd_create, (long)name, flags);
}
