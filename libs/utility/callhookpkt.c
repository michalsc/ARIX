/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <utility/hooks.h>
#include <clib/utility_protos.h>

uintptr_t CallHookPkt(struct Hook *hook, void * object, void * paramPacket)
{
    if (hook != NULL && hook->h_Entry != NULL)
        return hook->h_Entry(hook, object, paramPacket);
    else
        return 0;
}