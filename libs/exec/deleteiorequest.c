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

void DeleteIORequest(struct IORequest *io)
{
    if (io != NULL)
        FreeMem(io, io->io_Message.mn_Length);
}
