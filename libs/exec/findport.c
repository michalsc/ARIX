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
 *      FindPort - Find public message port by name
 *
 * SYNOPSIS
 *      uuid_t FindPort(const char *name);
 *
 * FUNCTION
 *      Searches public port list for given name and returns message port
 *      ID (uuid) which can be subsequently used to send messages to that
 *      port. Once ID is obtained, messages can be send to even if the
 *      owner has removed the message port from the public list.
 *
 * INPUTS
 *      name - A null-terminated string with public port name of interest.
 *
 * RESULT
 *      uuid_t - The ID of the message port or zero uuid if the port has not
 *               been found.
 *
 * SEE ALSO
 *      AddPort(), RemPort()
*/
uuid_t FindPort(const char * name)
{
    /* Port to ARIX master who stores public list of message ports */
    uuid_t arixPort = MAKE_UUID(0x00000001, 0x0000, 0x4000, 0x8000 | NT_MSGPORT, 0x000000000000);
    uuid_t id = MAKE_UUID(0, 0, 0, 0, 0);
    struct MsgARIXFindPort *msg = NULL;
    struct MsgPort *reply = NULL;
    int messageLength = 0;

    D(bug("[EXEC] FindPort('%s')\n", name));

    // Assert port name is given
    if (name)
    {
        // Calculate length of the entire message and allocate the buffer
        messageLength = strlen(name) + 1 + sizeof(struct MsgARIXFindPort);
        msg = (struct MsgARIXFindPort *)AllocVec(messageLength, MEMF_CLEAR);

        if (msg)
        {
            // Create temporary reply port
            reply = CreateMsgPort();

            if (reply)
            {
                struct MsgARIXFindPort *m = NULL;
                // Fill contents of the message
                msg->hdr.ma_Request = MSG_ARIX_FIND_PORT;
                msg->hdr.ma_Message.mn_ReplyPort = reply->mp_ID;
                msg->hdr.ma_Message.mn_Length = messageLength - sizeof(struct Message);
                CopyMem(name, msg->name, strlen(name));

                // Send message to ARIX
                D(bug("[EXEC] Sending message...\n"));
                PutMsg(arixPort, (struct Message *)msg);

                // Wait for an answer...
                D(bug("[EXEC] Waiting for reply...\n"));
                WaitPort(reply);
                m = (struct MsgARIXFindPort *)GetMsg(reply);

                // And store it
                id = m->port;

                D(bug("[EXEC] Got message %p\n", (void *)m));
                D(bug("[EXEC] FindPort() = {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
                       id.time_low, id.time_med, id.time_hi_and_version,
                       id.clock_seq_hi_and_reserved << 8 | id.clock_seq_low,
                       id.node[0], id.node[1], id.node[2],
                       id.node[3], id.node[4], id.node[5]));

                // Clean up
                DiscardMsg((struct Message *)m);
                DeleteMsgPort(reply);
            }
            FreeVec(msg);
        }
    }

    return id;
}
