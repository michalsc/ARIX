/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#define _GNU_SOURCE
#include <sys/syscall.h>
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
#define DEBUG
#include "exec_debug.h"

struct Message *GetMsg(struct MsgPort *port)
{
    struct Message *msg = NULL;

    if (port)
    {
        ObtainMutex(&port->mp_Lock);

        if (IsListEmpty(&port->mp_MsgList))
        {
            msg = InternalGetMsg(port);
        }
        else
        {
            msg = (struct Message *)REMHEAD(&port->mp_MsgList);
        }

        ReleaseMutex(&port->mp_Lock);
    }

    return msg;
}

struct Message *InternalGetMsg(struct MsgPort *port)
{
    struct Message * msg = NULL;

    if (port && port->mp_Socket > 0)
    {
        size_t msglength = offsetof(struct Message, mn_ReplyPort);
        struct iovec io;
        struct msghdr msghdr = {
            NULL, 0,        // msg_name, msg_namelen
            &io, 1,         // msg_iov, msg_iolen
            (void*)((intptr_t)port->mp_ReceiveBuffer + MESSAGE_MAX_LENGTH),
            MESSAGE_CTRL_MAX_LENGTH,
            0
        };

        msg = port->mp_ReceiveBuffer;

        io.iov_base = &msg->mn_ReplyPort;
        io.iov_len = 4096 - offsetof(struct Message, mn_ReplyPort);

        //int nbytes = recv(port->mp_Socket, &msg->mn_ReplyPort, 4096 - offsetof(struct Message, mn_ReplyPort), 0);
        int nbytes = syscall(SYS_recvmsg, port->mp_Socket, &msghdr, 0);
        if (nbytes <= 0)
        {
            return NULL;
        }
        msglength += nbytes;
        msg->mn_Owner = port;

        // Ancillary data received. attach it past the message data and update message headers
        // accordingly.
        if (msghdr.msg_controllen > 0) {
            D(bug("[EXEC] Control data of size %zu received\n", msghdr.msg_controllen));
            msg->mn_Control = (void *)((intptr_t)msg + msglength);
            msg->mn_ControlLength = msghdr.msg_controllen;
            CopyMem(msghdr.msg_control, msg->mn_Control, msg->mn_ControlLength);
            msglength += msg->mn_ControlLength;
        }

/*
        printf("[EXEC] Message %p with length of %d (actually read %d)\n", (void *)msg, msg->mn_Length, nbytes);
        printf("[EXEC] MessageType: %d\n", msg->mn_Type);
        printf("[EXEC] ReplyPort = {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
                msg->mn_ReplyPort.time_low, msg->mn_ReplyPort.time_med, msg->mn_ReplyPort.time_hi_and_version,
                msg->mn_ReplyPort.clock_seq_hi_and_reserved << 8 | msg->mn_ReplyPort.clock_seq_low,
                msg->mn_ReplyPort.node[0], msg->mn_ReplyPort.node[1], msg->mn_ReplyPort.node[2],
                msg->mn_ReplyPort.node[3], msg->mn_ReplyPort.node[4], msg->mn_ReplyPort.node[5]);
*/
        msg = ReallocPooled(port->mp_MsgPool, msg, msglength);

        port->mp_ReceiveBuffer = AllocVecPooled(port->mp_MsgPool, MESSAGE_MAX_LENGTH + MESSAGE_CTRL_MAX_LENGTH);
    }

    return msg;
}
