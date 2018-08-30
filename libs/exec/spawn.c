#define _GNU_SOURCE
#include <sys/syscall.h>
#include <utility/hooks.h>
#include <clib/exec_protos.h>
#include <sys/types.h>
#include <unistd.h>

void Spawn(struct Hook * spawnHook)
{
    if (spawnHook)
    {
        pid_t pid = (pid_t)syscall(SYS_fork); //fork();

        if (pid > 0)
        {
            spawnHook->h_Entry(spawnHook, &pid, NULL);
        }
    }
}