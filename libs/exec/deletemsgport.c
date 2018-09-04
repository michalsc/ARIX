/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
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
