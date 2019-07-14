#include <time.h>

void SubTime(struct timespec *dest, const struct timespec *src)
{
    /* Subtract src timespec from dest and store result in dest */
    dest->tv_nsec -= src->tv_nsec;
    dest->tv_sec -= src->tv_sec;

    /* If number of nanoseconds is negative, normalize the result */
    while (dest->tv_nsec < 0) {
        dest->tv_sec--;
        dest->tv_nsec += 1000000000;
    };

    /* 
        If, however, number of nanosecond is larger than or equal 1s, 
        normalize it the other direction
    */
    while (dest->tv_nsec >= 1000000000) {
        dest->tv_sec++;
        dest->tv_nsec -= 1000000000;
    }
}