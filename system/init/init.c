#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <linux/memfd.h>
#include <linux/fcntl.h>
#include <unistd.h>

static const char __attribute__((used)) version[] = "\0$VER: System/init 60.0 " VERSION_STRING_DATE;

void _start(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    char buf[10] = {0,};
    char *b = &buf[10];

    const char * arg[] = {
        "SYS:System/ARIX", (char *)0, (char *)0
    };

    const char * env[] = {
        "LD_LIBRARY_PATH=/ARIX/Libs", (char *)0
    };

    int mem = syscall(SYS_memfd_create, "hellomsg", MFD_ALLOW_SEALING);
    syscall(SYS_write, mem, "Hello!", 6);
    syscall(SYS_lseek, mem, 0, SEEK_SET);
    syscall(SYS_fcntl, mem, F_ADD_SEALS, F_SEAL_GROW | F_SEAL_SHRINK | F_SEAL_WRITE | F_SEAL_SEAL);

    *--b = 0;
    while (mem != 0)
    {
        *--b = '0' + (mem % 10);
        mem /= 10;
    }
    arg[1] = b;

    syscall(SYS_write, 1, "[INIT] Mounting proc filesystem\n", 32);
    syscall(SYS_mount, "proc", "/proc", "proc", 0, NULL);

    syscall(SYS_write, 1, "[INIT] Booting ARIX\n", 20);
    syscall(SYS_execve, "/ARIX/System/ARIX", arg, env);
    syscall(SYS_exit, 0);
}
