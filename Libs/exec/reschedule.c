#include <sched.h>

void Reschedule()
{
    sched_yield();
}
