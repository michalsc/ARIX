/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/nodes.h>
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
#include <strings.h>
#include "exec_intern.h"

void InternalPutMsg(struct ID portID, struct Message *msg);

void ReplyMsg(struct Message * msg)
{
    if (msg)
    {
        uint64_t *s = (uint64_t *)&msg->mn_ReplyPort;
        if (s[0] || s[1])
        {
            msg->mn_Type = NT_REPLYMSG;
            InternalPutMsg(msg->mn_ReplyPort, msg);
        }

        DiscardMsg(msg);
    }
}
