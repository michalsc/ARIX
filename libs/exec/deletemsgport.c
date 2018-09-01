
#include <exec/types.h>
#include <exec/ports.h>
#include <exec/memory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <clib/exec_protos.h>
#include <fcntl.h>
#include <stdio.h>

#include "exec_intern.h"

void DeleteMsgPort(struct MsgPort * port)
{
    printf("[EXEC] DeleteMsgPort(%p)\n", (void *)port);

    if (port)
    {
        close(port->mp_Socket);
        DeletePool(port->mp_MsgPool);
        FreeMem(port, sizeof(struct MsgPort));
    }
}
