#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return sc3(SYS_bind, sockfd, (long)addr, addrlen);
}
