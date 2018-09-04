/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <unistd.h>
#include <stdlib.h>
#include <clib/exec_protos.h>
#include <poll.h>
#include <stdio.h>

#include "exec_intern.h"

void WaitPort(struct MsgPort *port)
{
//    printf("[EXEC] WaitPort(%p)\n", port);

    if (port && port->mp_Socket > 0)
    {
        struct pollfd p[1] = {
            {port->mp_Socket, POLLIN, 0}};

        while(1) 
        {
            if (poll(p, 1, -1) > 0)
            {
//                printf("[EXEC] There's something in the port, WaitPort will return now\n");

                if (p[0].revents & POLLIN)
                {
                    break;
                }
            }
        }
    }
}