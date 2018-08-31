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

void InternalPutMsg(struct MsgPort *port, struct Message *msg);

static uuid_t empty_uuid = { 0, };

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
struct timespec t1, t2, t3, t4, t5, t0;
    clock_gettime(CLOCK_REALTIME, &t0);
        /* Create socket on which to send. */
        sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    clock_gettime(CLOCK_REALTIME, &t1);
        //        printf("[EXEC] socket=%d\n", sock);

        name.sun_family = AF_UNIX;
        name.sun_path[0] = 0;
        uuid_t *u = (uuid_t *)&name.sun_path[1];
        *u = port->mp_ID;
        //CopyMem(&port->mp_ID, &name.sun_path[1], sizeof(uuid_t));
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
    clock_gettime(CLOCK_REALTIME, &t2);
        struct iovec io[2] = {
            { &empty_uuid, sizeof(uuid_t) },
            { &msg->mn_Type, msg->mn_Length + 4 },
        };
        if (msg->mn_ReplyPort)
            io[0].iov_base = &msg->mn_ReplyPort->mp_ID;

        int ret = writev(sock, io, 2);
    clock_gettime(CLOCK_REALTIME, &t3);
        //        int ret =
        //        sendto(sock, msg, msg->mn_Length, 0,
        //               (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(uuid_t));

        //        printf("[EXEC] sendto returned %d\n", ret);

        close(sock);
    clock_gettime(CLOCK_REALTIME, &t4);
    int64_t d1, d2, d3, d4;
    d1 = t1.tv_nsec - t0.tv_nsec;
    d2 = t2.tv_nsec - t1.tv_nsec;
    d3 = t3.tv_nsec - t2.tv_nsec;
    d4 = t4.tv_nsec - t3.tv_nsec;
    printf("[EXEC] PutMsg: %d %d %d %d\n", d1, d2, d3, d4);
    }
}