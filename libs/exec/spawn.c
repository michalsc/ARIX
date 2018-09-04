/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

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