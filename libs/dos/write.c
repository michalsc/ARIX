/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <unistd.h>
#include <clib/dos_protos.h>

size_t Write(uuid_t handle, const void *buffer, size_t length)
{
    (void)handle;
    (void)buffer;
    (void)length;
    return 0;
}