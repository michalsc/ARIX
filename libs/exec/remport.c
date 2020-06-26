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
#include "exec_intern.h"
#include <string.h>
#include <strings.h>

#define DEBUG
#include "exec_debug.h"

/**
 * NAME
 *      RemPort - Remove the message port from the list of public ports
 *
 * SYNOPSIS
 *      void RemPort(struct MsgPort *port);
 *
 * FUNCTION
 *      Removes given message port from the list of public ports. After
 *      this call subsequent attempts to find the port using its name
 *      will fail. The message port is still active though and
 *      applications which already obtained its ID may still continue to
 *      use it.
 *
 * INPUTS
 *      port - Message port to be removed from the public port list.
 *
 * NOTE
 *      Further versions of exec.library may remove the port automatically
 *      upon application exit or deletion of message port.
 *
 * SEE ALSO
 *      FindPort(), AddPort(), CreateMsgPort(), DeleteMsgPort()
*/
void RemPort(struct MsgPort *port)
{
    /* Port to ARIX master who stores public list of message ports */
    struct ID arixPort = ARIX_PORT_ID;
    struct MsgARIXRemPort _msg, *msg = &_msg;
    struct MsgPort *reply = NULL;

    D(bug("[EXEC] RemPort(%p)\n", (void *)port));

    // Create reply port
    reply = CreateMsgPort();

    if (reply)
    {
        // Fill the message
        msg->hdr.ma_Request = MSG_ARIX_REM_PORT;
        msg->hdr.ma_Message.mn_Control = NULL;
        msg->hdr.ma_Message.mn_ReplyPort = reply->mp_ID;
        msg->hdr.ma_Message.mn_Length = sizeof(struct MsgARIXRemPort) - sizeof(struct Message);
        CopyMem(&port->mp_ID, &msg->port, sizeof(uuid_t));

        // Send message to ARIX
        D(bug("[EXEC] Sending message...\n"));
        PutMsg(arixPort, (struct Message *)msg);

        // And wait for a reply
        D(bug("[EXEC] Waiting for reply...\n"));
        WaitPort(reply);
        msg = (struct MsgARIXRemPort *)GetMsg(reply);

        // All done. Clean up the mess (reply port and received message)
        D(bug("[EXEC] Got message %p\n", (void *)msg));
        D(bug("[EXEC] RemPort() = %d\n", (int)msg->hdr.ma_RetVal));

        DiscardMsg((struct Message *)msg);
        DeleteMsgPort(reply);
    }
}
