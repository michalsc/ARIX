#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <clib/timer_protos.h>
#include <time.h>
#include <proto/kernel.h>

void NanoTime(const struct timespec *time)
{
    struct timespec now = { 0, 0 };

    SC_clock_gettime(CLOCK_MONOTONIC, &now);
    AddTime(&now, time);
    
    while(SC_clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &now, NULL) != 0);
}
