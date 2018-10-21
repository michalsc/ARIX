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

void DeletePool(void * p)
{
    struct PrivPool *pool = p;

    if (pool && ObtainMutex(&pool->mutex))
    {
        tlsf_destroy(pool->handle);
        FreeMem(pool, sizeof(struct PrivPool));
    }
}