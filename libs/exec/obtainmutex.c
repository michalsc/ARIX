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

int ObtainMutex(APTR mutex)
{
    struct Mutex *m = (struct Mutex *)mutex;

    bug("[EXEC] AcquireMutex(%p)\n", m);

    /* Make sure the Mutex is really a mutex */
    if (m != NULL && m->m_Node.ln_Type == NT_MUTEX)
    {
        /* Repeat forever */
        while(1)
        {
            int err;

            /* 
                Test if Mutex is free, in that case set it to locked state. In case of
                a success, the builtin returns true, in case of failure false is returned.
             */
            if (__sync_bool_compare_and_swap(&m->m_Lock, MUTEX_FREE, MUTEX_LOCKED))
                break;

            /*
                Attempt to obtain mutex failed. Call FUTEX_WAIT. The system call will wait
                if and only if the futex is still locked, otherwise it returns immediately
                so that another lock attempt can be made.
            */
            err = syscall(SYS_futex, FUTEX_WAIT, MUTEX_LOCKED, NULL, NULL, 0);
            
            /* Error was there? Report a failure */
            if (err < 0 && err != -EAGAIN)
            {
                bug("[EXEC] error waiting for futex\n");
                return FALSE;
            }
        }
    }
    else
    {
        /* Wasn't that a mutex? Return FALSE */
        bug("[EXEC] Not a mutex!\n");
        return FALSE;
    }

    return TRUE;
}
