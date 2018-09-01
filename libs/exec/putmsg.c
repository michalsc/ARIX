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
#include <time.h>

#define D(x) x

void InternalPutMsg(uuid_t portID, struct Message *msg);

void PutMsg(uuid_t portID, struct Message * msg)
{
    msg->mn_Type = NT_MESSAGE;
    InternalPutMsg(portID, msg);
}

void InternalPutMsg(uuid_t portID, struct Message *msg)
{
//    printf("[EXEC] PutMsg(%p, %p)\n", port, msg);

    if (msg)
    {
        struct sockaddr_un name = { AF_UNIX, {0,} };
        uuid_t *u = (uuid_t *)&name.sun_path[1];

        msg->mn_Pad = 0;
        *u = portID;

/*
        printf("[EXEC] Sending Message with length of %d\n", msg->mn_Length);
        printf("[EXEC] MessageType: %d\n", msg->mn_Type);
        printf("[EXEC] TargetPort = {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
               port->mp_ID.time_low, port->mp_ID.time_med, port->mp_ID.time_hi_and_version,
               port->mp_ID.clock_seq_hi_and_reserved << 8 | port->mp_ID.clock_seq_low,
               port->mp_ID.node[0], port->mp_ID.node[1], port->mp_ID.node[2],
               port->mp_ID.node[3], port->mp_ID.node[4], port->mp_ID.node[5]);

        printf("[EXEC] ReplyPort = {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
               msg->mn_ReplyPort->mp_ID.time_low, msg->mn_ReplyPort->mp_ID.time_med, msg->mn_ReplyPort->mp_ID.time_hi_and_version,
               msg->mn_ReplyPort->mp_ID.clock_seq_hi_and_reserved << 8 | msg->mn_ReplyPort->mp_ID.clock_seq_low,
               msg->mn_ReplyPort->mp_ID.node[0], msg->mn_ReplyPort->mp_ID.node[1], msg->mn_ReplyPort->mp_ID.node[2],
               msg->mn_ReplyPort->mp_ID.node[3], msg->mn_ReplyPort->mp_ID.node[4], msg->mn_ReplyPort->mp_ID.node[5]);
*/

        sendto(OutSocket, 
            &msg->mn_ReplyPort, msg->mn_Length + sizeof(struct Message) - offsetof(struct Message, mn_ReplyPort), 0, 
            (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(uuid_t)
        );

//        printf("[EXEC] sendto returned %d\n", ret);
    }
}