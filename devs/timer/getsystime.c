#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>

void GetSysTime(struct timespec *time)
{
    if (time == NULL)
        return;

    syscall(SYS_clock_gettime, CLOCK_REALTIME, time);
}
