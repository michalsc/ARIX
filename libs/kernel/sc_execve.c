#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_execve(const char *pathname, const char ** argv, const char ** envp)
{
    return sc3(SYS_execve, (long)pathname, (long)argv, (long)envp);
}
