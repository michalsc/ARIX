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

uuid_t FindPort(const char * name)
{
    struct MsgPort arix = {
        MAKE_UUID(0x00000001, 0x0000, 0x4000, 0x8000 | NT_MSGPORT, 0x000000000000),
        0, NULL, NULL
    };
    uuid_t id = MAKE_UUID(0, 0, 0, 0, 0);
    struct MsgPort *reply = CreateMsgPort();
    int len = strlen(name) + 1 + sizeof(struct MsgARIXFindPort);
    struct MsgARIXFindPort *msg = (struct MsgARIXFindPort * )AllocVec(len, 0);
    bzero(msg, len);

    printf("[EXEC] FindPort('%s')\n", name);

    msg->hdr.ma_Message.mn_ReplyPort = reply;
    msg->hdr.ma_Message.mn_Length = len - sizeof(struct Message);
    CopyMem(name, msg->name, strlen(name));

    msg->hdr.ma_Request = MSG_ARIX_FIND_PORT;

    printf("[EXEC] Sending message...\n");

    PutMsg(&arix, (struct Message *)msg);
    FreeVec(msg);

    printf("[EXEC] Waiting for reply...\n");
    WaitPort(reply);
    msg = (struct MsgARIXFindPort *)GetMsg(reply);
    id = msg->port;

    printf("[EXEC] Got message %p\n", (void*)msg);
    printf("[EXEC] FindPort() = {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
                id.time_low, id.time_med, id.time_hi_and_version,
                id.clock_seq_hi_and_reserved << 8 | id.clock_seq_low,
                id.node[0], id.node[1], id.node[2],
                id.node[3], id.node[4], id.node[5]);

    DiscardMsg((struct Message *)msg);

    return id;
}
