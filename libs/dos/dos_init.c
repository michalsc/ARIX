/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/libraries.h>
#include <clib/dos_protos.h>
#include <proto/exec.h>

#include <linux/limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

extern struct Library *DOSBase;
static void *_handle = NULL;

struct Library *LibOpen(void *h, uint32_t version)
{
    (void)version;
    _handle = h;
    return DOSBase;
}

void *LibClose()
{
    return _handle;
}

void *LibExpunge()
{
    return NULL;
}

void *GetHandle()
{
    return _handle;
}

char *__pr_HomePath;
intptr_t __pr_result_2;
int __pr_HomeDir;

void __attribute__((constructor)) DOSInit()
{
    printf("[DOS] DOSInit()\n");

    /* Get path to the file */
    __pr_HomePath = AllocVec(PATH_MAX, MEMF_CLEAR);
    size_t path_length = syscall(SYS_readlink, "/proc/self/exe", __pr_HomePath, PATH_MAX);

    /* Remove executable name from the path */
    while(path_length > 0 && __pr_HomePath[path_length] != '/')
    {
        __pr_HomePath[path_length--] = 0;
    }

    /* Release as much memory as possible */
    __pr_HomePath = ReallocMem(__pr_HomePath, path_length + 1);

    printf("[DOS] HomePath = %s\n", __pr_HomePath);

    __pr_HomeDir = syscall(SYS_open, __pr_HomePath, O_RDONLY);

    printf("[DOS] PROGDIR: lock = %d\n", __pr_HomeDir);
}
