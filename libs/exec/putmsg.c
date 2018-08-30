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

void InternalPutMsg(struct MsgPort *port, struct Message *msg);

void PutMsg(struct MsgPort * port, struct Message * msg)
{
    msg->mn_Type = NT_MESSAGE;
    InternalPutMsg(port, msg);
}

void InternalPutMsg(struct MsgPort *port, struct Message *msg)
{
//    printf("[EXEC] PutMsg(%p, %p)\n", port, msg);

    if (port && msg)
    {
        int sock;
        struct sockaddr_un name;

        msg->mn_Pad = 0;

        /* Create socket on which to send. */
        sock = socket(AF_UNIX, SOCK_DGRAM, 0);

//        printf("[EXEC] socket=%d\n", sock);

        name.sun_family = AF_UNIX;
        name.sun_path[0] = 0;
        CopyMem(&port->mp_ID, &name.sun_path[1], sizeof(uuid_t));
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
        connect(sock, (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(uuid_t));

        struct iovec io[2] = {
            { &msg->mn_ReplyPort->mp_ID, sizeof(uuid_t) },
            { &msg->mn_Type, msg->mn_Length + 4 },
        };

        int ret = writev(sock, io, 2);

//        int ret = 
//        sendto(sock, msg, msg->mn_Length, 0,
//               (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(uuid_t));

//        printf("[EXEC] sendto returned %d\n", ret);

        close(sock);
    }
}