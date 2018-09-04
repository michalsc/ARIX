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

void RemPort(struct MsgPort *port)
{
    uuid_t arixPort = MAKE_UUID(0x00000001, 0x0000, 0x4000, 0x8000 | NT_MSGPORT, 0x000000000000);
    struct MsgPort *reply = CreateMsgPort();
    struct MsgARIXRemPort *msg = (struct MsgARIXRemPort * )AllocVec(sizeof(struct MsgARIXRemPort), MEMF_CLEAR);

    printf("[EXEC] RemPort(%p)\n", (void*)port);

    msg->hdr.ma_Message.mn_ReplyPort = reply->mp_ID;
    msg->hdr.ma_Message.mn_Length = sizeof(struct MsgARIXRemPort) - sizeof(struct Message);
    CopyMem(&port->mp_ID, &msg->port, sizeof(uuid_t));

    msg->hdr.ma_Request = MSG_ARIX_REM_PORT;

    printf("[EXEC] Sending message...\n");

    PutMsg(arixPort, (struct Message *)msg);
    FreeVec(msg);

    printf("[EXEC] Waiting for reply...\n");
    WaitPort(reply);
    msg = (struct MsgARIXRemPort *)GetMsg(reply);

    printf("[EXEC] Got message %p\n", (void*)msg);
    printf("[EXEC] RemPort() = %d\n", (int)msg->hdr.ma_RetVal);

    DiscardMsg((struct Message *)msg);
    DeleteMsgPort(reply);
}
