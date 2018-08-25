#include <sched.h>
#include <clib/exec_protos.h>

void Reschedule()
{
    sched_yield();
}
