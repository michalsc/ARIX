#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

ssize_t SC_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
    return sc6(SYS_sendto, sockfd, (long)buf, len, flags, (long)dest_addr, (long)addrlen);
}

ssize_t SC_send(int sockfd, const void *buf, size_t len, int flags)
{
    return sc6(SYS_sendto, sockfd, (long)buf, len, flags, 0, 0);
}
