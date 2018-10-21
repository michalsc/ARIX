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
#include <linux/futex.h>
#include <errno.h>

#include <exec/mutex.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"

int AttemptMutex(APTR mutex)
{
    struct Mutex *m = (struct Mutex *)mutex;

    bug("[EXEC] TryMutex(%p)\n", m);

    if (m != NULL && m->m_Node.ln_Type == NT_MUTEX)
    {
        if (__sync_bool_compare_and_swap(&m->m_Lock, 1, 0))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        bug("[EXEC] Not a mutex!\n");
        return 0;
    }

    return 1;
}
