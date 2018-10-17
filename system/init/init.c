#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <unistd.h>

void _start(int argc, char **argv)
{
    const char * arg[] = {
        "ARIX", (char *)0
    };

    const char * env[] = {
        "LD_LIBRARY_PATH=/ARIX/Libs", (char *)0
    };

    syscall(SYS_write, 1, "Booting ARIX\n", 13);

    syscall(SYS_execve, "/ARIX/System/ARIX", arg, env);
    syscall(SYS_exit, 0);
}
