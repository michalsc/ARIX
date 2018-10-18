#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <unistd.h>

static const char __attribute__((used)) version[] = "\0$VER: System/init 60.0 " VERSION_STRING_DATE;

void _start(int argc, char **argv)
{
    const char * arg[] = {
        "ARIX", (char *)0
    };

    const char * env[] = {
        "LD_LIBRARY_PATH=/ARIX/Libs", (char *)0
    };

    syscall(SYS_write, 1, "[INIT] Mounting proc filesystem\n", 32);
    syscall(SYS_mount, "proc", "/proc", "proc", 0, NULL);

    syscall(SYS_write, 1, "[INIT] Booting ARIX\n", 20);
    syscall(SYS_execve, "/ARIX/System/ARIX", arg, env);
    syscall(SYS_exit, 0);
}
