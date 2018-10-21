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

void *ReallocPooled(void * p, void * memoryBLock, size_t newSize)
{
    struct PrivPool *pool = p;
    
    void *ret = NULL;

    if (pool && ObtainMutex(&pool->mutex))
    {
        ret = tlsf_realloc(pool->handle, memoryBLock, newSize);
        ReleaseMutex(&pool->mutex);
    }

    return ret;
}
