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
#include <sys/syscall.h>

#include <fcntl.h>
#include <stdio.h>
#include <clib/dos_protos.h>

#include "dos_private.h"

int AssignPath(const char * name, const char * path)
{
    printf("[DOS] AssignPath(%s, %s)\n", name, path);

    syscall(SYS_mkdirat, __AssignsDirLock, name, 0777);
    int fd = syscall(SYS_openat, __AssignsDirLock, name, O_RDONLY);
    printf("fd=%d\n", fd);
    syscall(SYS_symlinkat, path, fd, "0");
}
