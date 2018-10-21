/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <exec/types.h>
#include <exec/memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"
#include "tlsf.h"

void *AllocMemAligned(size_t byteSize, size_t align, uint32_t requirements)
{
    void *ptr = NULL;

    ObtainMutex(&local_memory_lock);
    ptr = tlsf_malloc_aligned(local_memory_pool, byteSize, align, &requirements);
    ReleaseMutex(&local_memory_lock);

    return ptr;
}
