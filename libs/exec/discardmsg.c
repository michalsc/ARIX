#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <unistd.h>
#include <stdlib.h>
#include <clib/exec_protos.h>
#include <poll.h>
#include <stdio.h>
#include <sys/uio.h>

#include "exec_intern.h"

void DiscardMsg(struct Message * msg)
{
    if (msg && msg->mn_ReceivePort)
    {
        if (((intptr_t)msg - (intptr_t)msg->mn_ReplyPort) == sizeof(uuid_t))
            FreeVecPooled(msg->mn_ReceivePort->mp_MsgPool, (void *)((intptr_t)msg - sizeof(uuid_t)));
        else
            FreeVecPooled(msg->mn_ReceivePort->mp_MsgPool, msg);
    }
}
