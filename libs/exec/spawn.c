#include <utility/hooks.h>
#include <clib/exec_protos.h>
#include <sys/types.h>
#include <unistd.h>

void Spawn(struct Hook * spawnHook)
{
    if (spawnHook)
    {
        pid_t pid = fork();

        if (pid > 0)
        {
            spawnHook->h_Entry(spawnHook, &pid, NULL);
        }
    }
}