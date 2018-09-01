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
    if (msg && msg->mn_Owner)
    {
        FreeVecPooled(msg->mn_Owner->mp_MsgPool, msg);
    }
}
