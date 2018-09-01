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
}
