/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <unistd.h>
#include <stdlib.h>
#include <clib/exec_protos.h>
#include <poll.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/uio.h>

#include "../../system/arix/arix_messages.h"
#include <string.h>
#include <strings.h>

#define DEBUG
#include "exec_debug.h"

/**
 * NAME
 *      AddPort - Add the message port to the list of public ports
 *
 * SYNOPSIS
 *      int AddPort(struct MsgPort *port, const char *name);
 *
 * FUNCTION
 *      Adds given message port to the list of public ports and advertise
 *      it unter specified name. After this call any other application can
 *      find the message port by the name and thus send messages to it.
 *
 *      The message ports in ARIX are identified by type 4 (random) uuid
 *      generated every time the CreateMsgPort function is called. The
 *      port IDs may vary even for the same application started twice or
 *      in parallel. Since a random guess of a 128 bit value is rather hard,
 *      public ports need to advertise themself by other means, here, a
 *      name.
 *
 *      Since each public port name has to be unique, the port must be
 *      removed form the public port list by a RemPort call as soon as
 *      port is deleted.
 *
 * INPUTS
 *      port - Message port to be added to the public port list.
 *      name - A null-terminated string with public port name.
 *
 * RESULT
 *      != 0 - The message port has been successfuly added to the list
 *      == 0 - The message port could not be added, probably there is
 *             A port registerred under given name already
 *
 * NOTE
 *      Further versions of exec.library may remove the port automatically
 *      upon application exit.
 *
 * SEE ALSO
 *      FindPort(), RemPort(), CreateMsgPort(), DeleteMsgPort()
 */
int AddPort(struct MsgPort *port, const char * name)
{
    /* Port to ARIX master who stores public list of message ports */
    struct ID arixPort = ARIX_PORT_ID;
    struct MsgARIXAddPort *msg = NULL;
    struct MsgPort *reply = NULL;
    int messageLength = 0;
    int retval = 0;

    D(bug("[EXEC] AddPort(%p, '%s')\n", (void *)port, name));

    // Assert port and name are given
    if (port != NULL && name != NULL)
    {
        // Calculate length of the message and allocate the buffer
        messageLength = strlen(name) + 1 + sizeof(struct MsgARIXAddPort);
        msg = (struct MsgARIXAddPort * )AllocVec(messageLength, MEMF_CLEAR);

        if (msg)
        {
            // Create temporary port
            reply = CreateMsgPort();

            if (reply)
            {
                struct MsgARIXAddPort * m = NULL;
                // Fill the message
                msg->hdr.ma_Request = MSG_ARIX_ADD_PORT;
                msg->hdr.ma_Message.mn_ReplyPort = reply->mp_ID;
                msg->hdr.ma_Message.mn_Length = messageLength - sizeof(struct Message);
                CopyMem(name, msg->name, strlen(name));
                CopyMem(&port->mp_ID, &msg->port, sizeof(uuid_t));

                // Send the message to ARIX port
                D(bug("[EXEC] Sending message...\n"));
                PutMsg(arixPort, (struct Message *)msg);

                // Expect a reply from ARIX server. It should tell us
                // if the port has been successfuly added to the list
                D(bug("[EXEC] Waiting for reply...\n"));
                WaitPort(reply);
                m = (struct MsgARIXAddPort *)GetMsg(reply);

                D(bug("[EXEC] Got message %p\n", (void*)m));
                D(bug("[EXEC] AddPort() = %d\n", (int)m->hdr.ma_RetVal));

                // Store the return value from ARIX
                retval = m->hdr.ma_RetVal;

                // Free the message and destroy reply port
                DiscardMsg((struct Message *)m);
                DeleteMsgPort(reply);
            }

            FreeVec(msg);
        }
    }

    return retval;
}
