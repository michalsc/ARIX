#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

int SC_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return sc3(SYS_connect, sockfd, (long)addr, addrlen);
}
