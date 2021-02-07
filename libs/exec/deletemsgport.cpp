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

#include "exec_debug.h"
#include "exec_intern.h"

/**
 * NAME
 *      DeleteMsgPort - Deallocate the message port
 *
 * SYNOPSIS
 *      void DeleteMsgPort(struct MsgPort * port);
 *
 * FUNCTION
 *      Disconnects the unix socket from message port and closes the
 *      descriptor. Subsequently releases the memory for incomming messages
 *      and for the message port itself.
 *
 *      It is safe to call this function with port == NULL.
 *
 * INPUTS
 *      port - Message port to be deleted or NULL.
 *
 * SEE ALSO
 *      CreateMsgPort(), AddPort(), RemPort()
 */
void DeleteMsgPort(struct MsgPort * port)
{
    D(bug("[EXEC] DeleteMsgPort(%p)\n", (void *)port));

    // Make sure non-null value was given.
    if (port)
    {
        // Remove port from the list
        __ports.remove(port);

        // Close unix socket
        SC_close(port->mp_Socket);

        // And release the memory
        DeletePool(port->mp_MsgPool);
        FreeMem(port, sizeof(struct MsgPort));
    }
}
