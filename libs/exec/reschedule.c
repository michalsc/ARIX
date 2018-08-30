#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <clib/exec_protos.h>

void Reschedule()
{
    syscall(SYS_sched_yield);
    //sched_yield();
}
 