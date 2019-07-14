#include <time.h>

void AddTime(struct timespec *dest, const struct timespec *src)
{
    dest->tv_sec += src->tv_sec;
    dest->tv_nsec += src->tv_nsec;

    /* Normalize the time */
    while (dest->tv_nsec >= 1000000000) {
        dest->tv_sec++;
        dest->tv_nsec -= 1000000000;
    }
}