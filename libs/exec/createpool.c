/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <exec/types.h>
#include <exec/memory.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

static APTR fetch_more_ram(void *data, IPTR *size)
{
    struct PrivPool *p = data;
    APTR ptr = AllocMem(*size, p->requirements);
    return ptr;
}

static void release_ram(void *data, APTR ptr, IPTR size)
{
    (void)data;
    FreeMem(ptr, size);
}

void *CreatePool(uint32_t requirement, size_t puddleSize, size_t threshSize)
{
    (void)threshSize;
    struct PrivPool * pool = AllocMem(sizeof(struct PrivPool), MEMF_CLEAR);

    if (pool)
    {
        pool->requirements = requirement;
        pool->handle = tlsf_init_autogrow(puddleSize, requirement, fetch_more_ram, release_ram, pool);
    }

    return pool;
}