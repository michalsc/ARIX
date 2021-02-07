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
//#include <sys/syscall.h>

#include <exec/types.h>
#include <exec/ports.h>
#include <exec/memory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <clib/exec_protos.h>
#include <fcntl.h>
#include <stdio.h>

#include <proto/kernel.h>

#define DEBUG
#include "exec_debug.h"
#include "exec_intern.h"

/**
 * NAME
 *      CreateMsgPort - Allocate and initialize a message port
 *
 * SYNOPSIS
 *      struct MsgPort * CreateMsgPort(void);
 *
 * FUNCTION
 *      Allocates memory for the message port and creates local memory
 *      pool for message reception. Finally, random MsgPort ID is created
 *      either using kernel's random number generator or pseudo R.N.G.
 *      fallback. Finally, unix socket for incoming messages is bound to
 *      the ID.
 *
 *      You *must* delete the message port by calling DeleteMsgPort()!
 *
 * RESULT
 *      MsgPort - Message port ready to receive incoming messages or NULL
 *              if either there was insufficient amount of memory or
 *              underlying unix socket could not be created or binding to
 *              the ID failed.
 *
 * SEE ALSO
 *      DeleteMsgPort(), AddPort(), RemPort()
 */
struct MsgPort * CreateMsgPort()
{
    struct sockaddr_un name;
    ID *sock_id = (ID *)&name.sun_path[1];
    struct MsgPort * port = static_cast<struct MsgPort *>(AllocMem(sizeof(struct MsgPort), MEMF_CLEAR));

    D(bug("[EXEC] CreateMsgPort()\n"));

    if (port)
    {
        // Initialize lock
        InitMutex(&port->mp_Lock, MUTEX_LOCKED);

        // Initialize list for incomming messages
        NEWLIST(&port->mp_MsgList);

        // Create socket
        port->mp_Socket = SC_socket(AF_UNIX, SOCK_DGRAM, 0);

        // If socket failed return NULL.
        if (port->mp_Socket < 0)
        {
            D(bug("[EXEC] Cannot create socket\n"));
            FreeMem(port, sizeof(struct MsgPort));
            return NULL;
        }

        // Put the socket in non-blocking mode.
        int flags = SC_fcntl(port->mp_Socket, F_GETFL, 0);
        SC_fcntl(port->mp_Socket, F_SETFL, flags | O_NONBLOCK);

        // Create memory pool for incomming messages.
        port->mp_MsgPool = CreatePool(0, 8192, 8192);
        if (port->mp_MsgPool == NULL)
        {
            D(bug("[EXEC] Cannot create memory pool\n"));
            SC_close(port->mp_Socket);
            FreeMem(port, sizeof(struct MsgPort));
            return NULL;
        }

        // Allocate first receive buffer of 4K size *in advance*
        port->mp_ReceiveBuffer = AllocVecPooled(port->mp_MsgPool, MESSAGE_MAX_LENGTH + MESSAGE_CTRL_MAX_LENGTH);

        // Attempt to create random ID for port and bind socket to that ID
        name.sun_family = AF_UNIX;
        name.sun_path[0] = 0;
        int maxtry = 1000;      // Even with bad random generator 1000 retries shoudl be enough
        int err = 0;

        do {
            // Get ID and copy it to the name
            port->mp_ID = GetID(NT_MSGPORT);
            *sock_id = port->mp_ID;

            // Number of retries exhausted? Just fail...
            if (--maxtry < 0)
            {
                D(bug("[EXEC] too many tries. giving up\n"));
                DeletePool(port->mp_MsgPool);
                SC_close(port->mp_Socket);
                FreeMem(port, sizeof(struct MsgPort));
                return NULL;
            }

            D(bug("[EXEC] Binding to port ID {%010llx-%02x-%04x}\n",
                port->mp_ID.raw >> 24, (port->mp_ID.raw >> 16) & 0xff, port->mp_ID.raw & 0xffff));

            // Try to bind socket to the ID in unix socket namespace.
            err = SC_bind(port->mp_Socket, (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(ID));
        } while (err != 0);

        // Unlock the port
        ReleaseMutex(&port->mp_Lock);
    }

    __ports.push_back(port);

    return port;
}
