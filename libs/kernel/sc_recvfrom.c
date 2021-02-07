#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

ssize_t SC_recvfrom(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *src_addr, socklen_t addrlen)
{
    return sc6(SYS_recvfrom, sockfd, (long)buf, len, flags, (long)src_addr, (long)addrlen);
}

ssize_t SC_recv(int sockfd, const void *buf, size_t len, int flags)
{
    return sc6(SYS_recvfrom, sockfd, (long)buf, len, flags, 0, 0);
}
