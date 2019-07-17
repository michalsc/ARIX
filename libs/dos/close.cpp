/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <clib/dos_protos.h>
#include <uuid/uuid.h>

#include "dos_private.h"
#include "arixfile.h"

int Close(uuid_t handle)
{
    int err = -EBADF;
    ARIXFile *f = NULL;
    uuid_t null = MAKE_UUID(0, 0, 0, 0, 0);

    if (handle == null)
        return TRUE;

    /* Check if the handle is in the list of open files */
    auto it = __files->find(handle);
    if (it != __files->end()) {
        f = it->second;
    }

    /* File handle found. Close the file and remove it from the list */
    if (f != NULL)
    {
        err = f->Close();
        __files->erase(it);
        delete f;
    }

    SetIoErr(-err);

    /* In case of error set __pr_result_2 and return FALSE */
    if (err)
    {
        return FALSE;
    }
    /* Return TRUE otherwise */
    else
    {
        return TRUE;
    }
}
