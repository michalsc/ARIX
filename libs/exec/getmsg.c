#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <unistd.h>
#include <stdlib.h>
#include <clib/exec_protos.h>
#include <poll.h>
#include <stdio.h>

#include "exec_intern.h"

struct Message * GetMsg(struct MsgPort * port)
{
    struct Message * msg = NULL;

    printf("[EXEC] GetMsg(%p)\n", port);

    if (port && port->mp_Socket > 0)
    {
        struct pollfd p[1] = {
            { port->mp_Socket, POLLIN, 0 }
        };

        if (poll(p, 1, 0) > 0)
        {
            printf("[EXEC] There's something in the port\n");

            if (p[0].revents & POLLIN) {
                struct Message * msg = AllocVecPooled(port->mp_MsgPool, 4096);
                int nbytes = read(port->mp_Socket, msg, 4096);

                printf("[EXEC] Message with length of %d (actually read %d)\n", msg->mn_Length, nbytes);
                printf("[EXEC] ReplyPort = {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
                       msg->mn_ReplyPort.time_low, msg->mn_ReplyPort.time_med, msg->mn_ReplyPort.time_hi_and_version,
                       msg->mn_ReplyPort.clock_seq_hi_and_reserved << 8 | msg->mn_ReplyPort.clock_seq_low,
                       msg->mn_ReplyPort.node[0], msg->mn_ReplyPort.node[1], msg->mn_ReplyPort.node[2],
                       msg->mn_ReplyPort.node[3], msg->mn_ReplyPort.node[4], msg->mn_ReplyPort.node[5]);
                
                ReallocVec(port->mp_MsgPool, msg, nbytes);
            }
        }
    }

    return msg;
}