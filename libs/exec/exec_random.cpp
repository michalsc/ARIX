/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <exec/types.h>
#include <exec/id.h>
#include <time.h>

#include "exec_random.h"
#include "exec_debug.h"

uint32_t RandomNumberGenerator::_seed = 0;

void RandomNumberGenerator::seed()
{ 
    struct timespec t;
    bug("[EXEC] Setting up random seed: ");
    syscall(SYS_clock_gettime, CLOCK_REALTIME, &t);
#if __SIZEOF_LONG__ > 4
    _seed = 2654435761 * ((t.tv_sec >> 32) ^ (t.tv_sec) ^ t.tv_nsec);
#else
    _seed = 2654435761 * ((t.tv_sec) ^ t.tv_nsec);
#endif
    bug("0x%08x\n", _seed);
}