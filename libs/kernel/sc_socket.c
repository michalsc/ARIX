#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_socket(int domain, int type, int protocol)
{
    return sc3(SYS_socket, domain, type, protocol);
}
