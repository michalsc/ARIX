#ifndef _DEVICES_TIMER_H
#define _DEVICES_TIMER_H

#include <exec/io.h>
#include <time.h>

struct timerequest {
    struct IORequest tr_node;
    struct timespec  tr_time;
};

#endif /* _DEVICES_TIMER_H */
