/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <unistd.h>
#include <clib/dos_protos.h>

#include "dos_debug.h"
#include "dos_private.h"
#include "arixfile.h"

size_t Read(uuid_t handle, void *buffer, size_t length)
{
    int err = -EBADF;
    size_t bytesRead = 0;
    ARIXFile *f = NULL;
    uuid_t null = MAKE_UUID(0, 0, 0, 0, 0);

    if (handle == null)
    {
        SetIoErr(-err);
        return -1;
    }

    auto it = __files->find(handle);
    if (it != __files->end()) {
        f = it->second;
    }

    if (f != NULL)
    {
        bytesRead = f->Read(buffer, length);
        err = f->lastError();
    }

    SetIoErr(-err);

    return bytesRead;
}
