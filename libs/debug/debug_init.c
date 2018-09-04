/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <exec/types.h>
#include <exec/libraries.h>
#include <clib/debug_protos.h>

#include <stdio.h>

extern struct Library *DebugBase;
static void *_handle = NULL;

struct Library *Open(void *h, uint32_t version)
{
    (void)version;
    _handle = h;
    return DebugBase;
}

void *Close()
{
    return _handle;
}

void *Expunge()
{
    return NULL;
}

void *GetHandle()
{
    return _handle;
}
