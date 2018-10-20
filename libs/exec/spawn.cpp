/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <sys/socket.h>
#include <utility/hooks.h>
#include <clib/exec_protos.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "exec_intern.h"

void Spawn(struct Hook * spawnHook)
{
    if (spawnHook)
    {
#ifndef SYS_fork
        pid_t pid = (pid_t)syscall(SYS_clone, SIGCHLD, 0);
#else
        pid_t pid = (pid_t)syscall(SYS_flock);
#endif

        if (pid > 0)
        {
            /* Regenerate random seed and start time */
            syscall(SYS_clock_gettime, CLOCK_REALTIME, &StartTime);
            printf("[EXEC] Spawn(): Start time: %ld.%09ld\n", StartTime.tv_sec, StartTime.tv_nsec);
            UUID_Seed = 2654435761 * ((StartTime.tv_sec >> 32) ^ (StartTime.tv_sec) ^ StartTime.tv_nsec);
            printf("[EXEC] Spawn(): UUID Random seed=0x%08x\n", UUID_Seed);

            if (!__ports.empty())
            {
                printf("[EXEC] Spawn(): Renaming all MsgPorts\n");

                for (auto port: __ports)
                {
                    struct sockaddr_un name;
                    uuid_t *sock_id = (uuid_t *)&name.sun_path[1];

                    syscall(SYS_close, port->mp_Socket);

                    port->mp_Socket = syscall(SYS_socket, AF_UNIX, SOCK_DGRAM, 0);

                    // If socket failed return NULL.
                    if (port->mp_Socket < 0)
                    {
                        printf("[EXEC] Spawn(): Cannot create socket\n");
                    }

                    // Put the socket in non-blocking mode.
                    int flags = syscall(SYS_fcntl, port->mp_Socket, F_GETFL);
                    syscall(SYS_fcntl, port->mp_Socket, F_SETFL, flags | O_NONBLOCK);

                    name.sun_family = AF_UNIX;
                    name.sun_path[0] = 0;
                    int maxtry = 1000; // Even with bad random generator 1000 retries shoudl be enough
                    int err = 0;

                    do
                    {
                        // Get ID and copy it to the name
                        port->mp_ID = GetRandomID(NT_MSGPORT);
                        *sock_id = port->mp_ID;

                        // Number of retries exhausted? Just fail...
                        if (--maxtry < 0)
                        {
                            printf("[EXEC] Spawn(): too many tries. giving up\n");
                            return;
                        }

                        printf("[EXEC] Spawn(): Binding to port ID {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
                               port->mp_ID.time_low, port->mp_ID.time_med, port->mp_ID.time_hi_and_version,
                               port->mp_ID.clock_seq_hi_and_reserved << 8 | port->mp_ID.clock_seq_low,
                               port->mp_ID.node[0], port->mp_ID.node[1], port->mp_ID.node[2],
                               port->mp_ID.node[3], port->mp_ID.node[4], port->mp_ID.node[5]);

                        // Try to bind socket to the ID in unix socket namespace.
                        err = syscall(SYS_bind, port->mp_Socket, (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(uuid_t));
                    } while (err != 0);
                }
            }

            /* Call spawn hook */
            spawnHook->h_Entry(spawnHook, &pid, NULL);
        }
    }
}