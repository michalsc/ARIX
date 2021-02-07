#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/kernel_protos.h>

#include "syscall.h"

ssize_t SC_sendmsg(int sockfd, const struct msghdr *msg, int flags)
{
    return sc3(SYS_sendmsg, sockfd, (long)msg, flags);
}
