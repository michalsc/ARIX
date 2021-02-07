#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

void * SC_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
#ifdef SYS_mmap2
    return (void *)sc6(SYS_mmap2, (long)addr, length, prot, flags, fd, offset);
#else
    return (void *)sc6(SYS_mmap, (long)addr, length, prot, flags, fd, offset);
#endif
}
