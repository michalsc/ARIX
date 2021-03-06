/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#undef _GNU_SOURCE
#define _GNU_SOURCE

#include <unistd.h>
#include <sys/mount.h>
#include <sys/mman.h>

#include <clib/kernel_protos.h>

static const char __attribute__((used)) version[] = "\0$VER: System/init 60.0 " VERSION_STRING_DATE;
int __loc_errno;
long __stack_chk_guard;
void __stack_chk_fail(void)
{
    SC_exit(-1);
}

void do_mounts()
{
    SC_mount("proc", "/proc", "proc", MS_NOSUID | MS_NOEXEC | MS_NODEV, NULL);
    SC_mount("devtmpfs", "/dev", "devtmpfs", MS_NOSUID, "mode=755");
    SC_mount("tmpfs", "/tmp", "tmpfs", MS_NODEV, NULL);

    SC_mkdir("/dev/pts", 0755);
    SC_mkdir("/dev/shm", 0777);

    SC_mount("sysfs", "/sys", "sysfs", MS_NOSUID | MS_NOEXEC | MS_NODEV, NULL);
    SC_mount("devpts", "/dev/pts", "devpts", MS_NOSUID | MS_NOEXEC, NULL);
    SC_mount("tmpfs", "/dev/shm", "tmpfs", MS_NOSUID | MS_NODEV, NULL);
}

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
        "LD_LIBRARY_PATH=/ARIX/Libs:/lib64:/lib", (char *)0
    };

    int mem = SC_memfd_create("hellomsg", MFD_ALLOW_SEALING);
    SC_write(mem, "Hello!", 6);
    SC_lseek(mem, 0, SEEK_SET);
    SC_fcntl(mem, F_ADD_SEALS, F_SEAL_GROW | F_SEAL_SHRINK | F_SEAL_WRITE | F_SEAL_SEAL);

    *--b = 0;
    while (mem != 0)
    {
        *--b = '0' + (mem % 10);
        mem /= 10;
    }
    arg[1] = b;

    //SC_write(1, "[INIT] Mounting filesystems\n", 28);
    do_mounts();

    //SC_write(1, "[INIT] Booting ARIX\n", 20);
    SC_execve("/ARIX/System/ARIX", (const char**)arg, (const char**)env);

    SC_exit(0);
}
