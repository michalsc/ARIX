/*
    Copyright © 2018-2019 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <exec/io.h>
#include <clib/exec_protos.h>

void * CreateIORequest(struct MsgPort *ioReplyPort, size_t size)
{
    struct IORequest *retval = NULL;

    /*
        It is legal to give null reply port or size of the IO Request smaller than 
        the basic IORequest structure, but then the function will just fail.
    */
    if (ioReplyPort == NULL || size < sizeof(struct IORequest))
        return NULL;

    retval = (struct IORequest *)AllocMem(size, MEMF_CLEAR);

    if (retval != NULL)
    {
        retval->io_Message.mn_Owner = ioReplyPort;
        retval->io_Message.mn_ReplyPort = ioReplyPort->mp_ID;
        retval->io_Message.mn_Length = size - sizeof(struct Message);
        retval->io_ID = GetID(NT_IOREQUEST);
    }

    return retval;
}