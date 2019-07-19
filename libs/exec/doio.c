/*
    Copyright © 2018-2019 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <exec/devices.h>
#include <exec/io.h>
#include <clib/exec_protos.h>

int32_t DoIO(struct IORequest *request)
{
    struct DeviceLVO *lvo = NULL;

    /* If no request given return with an error */
    if (request == NULL)
        return -1;
    
    /* Try to process the message quick, on caller's thread */
    request->io_Flags |= IOF_QUICK;

    if (request->io_Device == NULL) {
        request->io_Error = -1;
        return request->io_Error;
    }

    /* Get Device-like LVO table and perform the BeginIO call */
    lvo = (struct DeviceLVO *)request->io_Device->dd_Library.lib_LVOTable;
    if (lvo != NULL) {
        lvo->BeginIO(request);
    }

    /* If BeginIO has cleared the quick flag, wait for completion of the request */
    if ((request->io_Flags & IOF_QUICK) == 0) {
        WaitIO(request);
    }

    return request->io_Error;
}