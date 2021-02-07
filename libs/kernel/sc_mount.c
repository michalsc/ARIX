#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_mount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags,
             const void *data)
{
    return sc5(SYS_mount, (long)source, (long)target, (long)filesystemtype, (long)mountflags, (long)data);
}
