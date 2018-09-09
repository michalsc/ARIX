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
#include <sys/uio.h>
#include <sys/socket.h>
#include <time.h>

#include "exec_intern.h"

void InternalPutMsg(struct MsgPort *port, struct Message *msg);

struct Message *GetMsg(struct MsgPort *port)
{
    struct Message * msg = NULL;

    if (port && port->mp_Socket > 0)
    {
        msg = port->mp_ReceiveBuffer;

        int nbytes = recv(port->mp_Socket, &msg->mn_ReplyPort, 4096 - offsetof(struct Message, mn_ReplyPort), 0);
        if (nbytes <= 0)
        {
            return NULL;
        }
        msg->mn_Owner = port;
/*
        printf("[EXEC] Message %p with length of %d (actually read %d)\n", (void *)msg, msg->mn_Length, nbytes);
        printf("[EXEC] MessageType: %d\n", msg->mn_Type);
        printf("[EXEC] ReplyPort = {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
                msg->mn_ReplyPort.time_low, msg->mn_ReplyPort.time_med, msg->mn_ReplyPort.time_hi_and_version,
                msg->mn_ReplyPort.clock_seq_hi_and_reserved << 8 | msg->mn_ReplyPort.clock_seq_low,
                msg->mn_ReplyPort.node[0], msg->mn_ReplyPort.node[1], msg->mn_ReplyPort.node[2],
                msg->mn_ReplyPort.node[3], msg->mn_ReplyPort.node[4], msg->mn_ReplyPort.node[5]);
*/
        msg = ReallocPooled(port->mp_MsgPool, msg, offsetof(struct Message, mn_ReplyPort) + nbytes);

        port->mp_ReceiveBuffer = AllocVecPooled(port->mp_MsgPool, 4096);
    }

    return msg;
}
