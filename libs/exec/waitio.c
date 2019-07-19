/*
    Copyright © 2018-2019 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#define _GNU_SOURCE
#include <sys/syscall.h>
#include <poll.h>

#include <exec/devices.h>
#include <exec/io.h>
#include <clib/exec_protos.h>
#include "exec_intern.h"

/*
    Warning - message passing is not yet thread safe!!!
*/

int32_t WaitIO(struct IORequest *request)
{
    struct DeviceLVO *lvo = NULL;
    struct MsgPort *inputPort = NULL;
    struct IORequest *msg = NULL, *next = NULL;

    /* If no request given return with an error */
    if (request == NULL)
        return -1;

    /* Get Device-like LVO table */
    lvo = (struct DeviceLVO *)request->io_Device->dd_Library.lib_LVOTable;
    if (lvo == NULL)
        return -1;

    /* If message was quick there is no need waiting for it. Just return an error code from request. */
    if (request->io_Flags & IOF_QUICK)
        return request->io_Error;

    inputPort = request->io_Message.mn_Owner;

    /* First check if the IORequest is already in message queue of the port */
    ObtainMutex(&inputPort->mp_Lock);

    ForeachNodeSafe(&inputPort->mp_MsgList, msg, next)
    {
        if (UUID_EQUALS(msg->io_ID, request->io_ID))
        {
            /* Yes, there is message with the same ID, remove it from queue */
            REMOVE(msg);
            break;
        }
    }

    /* If no message with correct ID was found, the message wasn't replied yet. */
    while (msg == NULL)
    {
        do
        {
            /* Fetch all messages available in the MsgPort */
            msg = (struct IORequest *)InternalGetMsg(request->io_Message.mn_Owner);

            /* Got it? */
            if (msg != NULL)
            {
                /* Does the ID match? */
                if (!UUID_EQUALS(msg->io_ID, request->io_ID))
                {
                    /* No, add it to the queue of received messages */
                    ADDTAIL(&request->io_Message.mn_Owner->mp_MsgList, msg);
                    
                    /* Clear the msg pointer in order to indicate we're not done yet */
                    msg = NULL;
                }
            }
            else
            {
                /* There was no message, wait for new messages without calling WaitPort directly */
                
                struct pollfd p[1] = {
                    {request->io_Message.mn_Owner->mp_Socket, POLLIN, 0}};

                while(1)
                {
                    if (syscall(SYS_ppoll, p, 1, NULL, NULL) > 0)
                    {
                        if (p[0].revents & POLLIN)
                        {
                            break;
                        }
                    }
                }
            }
            
        } while(msg == NULL);
    }
    ReleaseMutex(&inputPort->mp_Lock);

    if (msg)
    {
        /* Copy over the message body back to the request */
        CopyMem(
            &msg->io_Message.mn_ReplyPort, 
            &msg->io_Message.mn_ReplyPort, 
            msg->io_Message.mn_Length + sizeof(struct Message) - offsetof(struct Message, mn_ReplyPort)
        );

        /* The replied message is not needed anymore. Discard it. */
        DiscardMsg((struct Message *)msg);

        /* Finalize the IO, i.e. let the device perform cleanup after request */
        lvo->FinalizeIO(request);
    }

    return request->io_Error;
}
