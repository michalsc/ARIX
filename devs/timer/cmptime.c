#include <time.h>

int CmpTime(const struct timespec *left, const struct timespec *right)
{
    /* Assume the time is normalized! */

    long diff = 0;

    if (left->tv_sec == right->tv_sec) {
        diff = left->tv_nsec - right->tv_nsec;
    } else {
        diff = left->tv_sec - right->tv_sec;
    }

    if (diff < 0)
        return -1;
    else if (diff > 0)
        return 1;
    else
        return 0;
}
