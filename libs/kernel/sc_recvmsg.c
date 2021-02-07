#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

ssize_t SC_recvmsg(int sockfd, const struct msghdr *msg, int flags)
{
    return sc3(SYS_recvmsg, sockfd, (long)msg, flags);
}
