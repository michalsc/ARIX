/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <exec/types.h>
#include <uuid/uuid.h>
#include <clib/uuid_protos.h>

void UUID_Copy(const uuid_t *src, uuid_t *dest)
{
    if (src != NULL && dest != NULL)
    {
        *dest = *src;
    }
}
