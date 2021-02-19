#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_symlinkat(const char *oldpath, int newdirfd, const char *newpath)
{
    return sc3(SYS_symlinkat, (long)oldpath, newdirfd, (long)newpath);
}