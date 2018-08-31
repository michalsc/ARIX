#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <unistd.h>
#include <stdlib.h>
#include <clib/exec_protos.h>
#include <poll.h>
#include <stdio.h>
#include <sys/uio.h>
#include <time.h>

#include "exec_intern.h"

void InternalPutMsg(struct MsgPort *port, struct Message *msg);

struct Message *GetMsg(struct MsgPort *port)
{
    struct Message * msg = NULL;

    if (port && port->mp_Socket > 0)
    {
        void *buff = port->mp_ReceiveBuffer; //AllocVecPooled(port->mp_MsgPool, 4096);
        msg = (struct Message *)((intptr_t)buff + sizeof(uuid_t));
        struct iovec io[2] = {
            { buff, sizeof(uuid_t) },
            { &msg->mn_Type, 4096 - sizeof(uuid_t) - offsetof(struct Message, mn_Type) },
        };
        int nbytes = readv(port->mp_Socket, io, 2);
        if (nbytes <= 0)
        {
            //FreeVecPooled(port->mp_MsgPool, buff);
            return NULL;
        }
        msg->mn_ReplyPort = buff;
        msg->mn_ReceivePort = port;
/*
        printf("[EXEC] Message %p with length of %d (actually read %d)\n", msg, msg->mn_Length, nbytes);
        printf("[EXEC] MessageType: %d\n", msg->mn_Type);
        printf("[EXEC] ReplyPort = {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
                msg->mn_ReplyPort->mp_ID.time_low, msg->mn_ReplyPort->mp_ID.time_med, msg->mn_ReplyPort->mp_ID.time_hi_and_version,
                msg->mn_ReplyPort->mp_ID.clock_seq_hi_and_reserved << 8 | msg->mn_ReplyPort->mp_ID.clock_seq_low,
                msg->mn_ReplyPort->mp_ID.node[0], msg->mn_ReplyPort->mp_ID.node[1], msg->mn_ReplyPort->mp_ID.node[2],
                msg->mn_ReplyPort->mp_ID.node[3], msg->mn_ReplyPort->mp_ID.node[4], msg->mn_ReplyPort->mp_ID.node[5]);
*/
        ReallocPooled(port->mp_MsgPool, buff, sizeof(uuid_t) + sizeof(struct Message) + msg->mn_Length);
        port->mp_ReceiveBuffer = AllocVecPooled(port->mp_MsgPool, 4096);
    }

    return msg;
}
