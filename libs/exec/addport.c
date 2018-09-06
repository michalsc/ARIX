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

#include "exec_intern.h"
#include <string.h>
#include <strings.h>

#define D(x) x

/**
 * NAME
 *      AddPort - Add the message port to the list of public ports
 * 
 * SYNOPSIS
 *      void AddPort(struct MsgPort *port, const char *name);
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
 * NOTE
 *      Further versions of exec.library may remove the port automatically
 *      upon application exit.
 * 
 * SEE ALSO
 *      RemPort, CreateMsgPort(), DeleteMsgPort()
*/
void AddPort(struct MsgPort *port, const char * name)
{
    uuid_t arixPort = MAKE_UUID(0x00000001, 0x0000, 0x4000, 0x8000 | NT_MSGPORT, 0x000000000000);
    struct MsgPort *reply = CreateMsgPort();
    int len = strlen(name) + 1 + sizeof(struct MsgARIXAddPort);
    struct MsgARIXAddPort *msg = (struct MsgARIXAddPort * )AllocVec(len, 0);
    bzero(msg, len);

    printf("[EXEC] AddPort(%p, '%s')\n", (void*)port, name);

    msg->hdr.ma_Message.mn_ReplyPort = reply->mp_ID;
    msg->hdr.ma_Message.mn_Length = len - sizeof(struct Message);
    CopyMem(name, msg->name, strlen(name));
    CopyMem(&port->mp_ID, &msg->port, sizeof(uuid_t));

    msg->hdr.ma_Request = MSG_ARIX_ADD_PORT;

    printf("[EXEC] Sending message...\n");

    PutMsg(arixPort, (struct Message *)msg);
    FreeVec(msg);

    printf("[EXEC] Waiting for reply...\n");
    WaitPort(reply);
    msg = (struct MsgARIXAddPort *)GetMsg(reply);

    printf("[EXEC] Got message %p\n", (void*)msg);
    printf("[EXEC] AddPort() = %d\n", (int)msg->hdr.ma_RetVal);

    DiscardMsg((struct Message *)msg);
    DeleteMsgPort(reply);
}
