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
#include <proto/kernel.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "exec_intern.h"
#define DEBUG
#include "exec_debug.h"
#include "exec_random.h"

void InitializeIDSeq();

void Spawn(struct Hook * spawnHook)
{
    if (spawnHook)
    {
        pid_t pid = SC_clone(SIGCHLD, 0, NULL, 0, NULL);

        if (pid > 0)
        {
            /* Regenerate random seed and start time */
            SC_clock_gettime(CLOCK_REALTIME, &StartTime);
            D(bug("[EXEC] Spawn(): Start time: %ld.%09ld\n", StartTime.tv_sec, StartTime.tv_nsec));
            RandomNumberGenerator::seed();
            InitializeIDSeq();

            if (!__ports.empty())
            {
                D(bug("[EXEC] Spawn(): Renaming all MsgPorts\n"));

                for (auto port: __ports)
                {
                    struct sockaddr_un name;
                    ID *sock_id = (ID *)&name.sun_path[1];

                    SC_close(port->mp_Socket);

                    port->mp_Socket = SC_socket(AF_UNIX, SOCK_DGRAM, 0);

                    // If socket failed return NULL.
                    if (port->mp_Socket < 0)
                    {
                        D(bug("[EXEC] Spawn(): Cannot create socket\n"));
                    }

                    // Put the socket in non-blocking mode.
                    int flags = SC_fcntl(port->mp_Socket, F_GETFL, 0);
                    SC_fcntl(port->mp_Socket, F_SETFL, flags | O_NONBLOCK);

                    name.sun_family = AF_UNIX;
                    name.sun_path[0] = 0;
                    int maxtry = 1000; // Even with bad random generator 1000 retries shoudl be enough
                    int err = 0;

                    do
                    {
                        // Get ID and copy it to the name
                        port->mp_ID = GetID(NT_MSGPORT);
                        *sock_id = port->mp_ID;

                        // Number of retries exhausted? Just fail...
                        if (--maxtry < 0)
                        {
                            D(bug("[EXEC] Spawn(): too many tries. giving up\n"));
                            return;
                        }

                        D(bug("[EXEC] Spawn(): Binding to port ID {%010llx-%02x-%04x}\n",
                               port->mp_ID.raw >> 24, (port->mp_ID.raw >> 16) & 0xff, port->mp_ID.raw & 0xffff));

                        // Try to bind socket to the ID in unix socket namespace.
                        err = SC_bind(port->mp_Socket, (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(ID));
                    } while (err != 0);
                }
            }

            /* Call spawn hook */
            spawnHook->h_Entry(spawnHook, &pid, NULL);
        }
    }
}
