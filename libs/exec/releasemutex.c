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

#define D(x) /* */

/**
 *  NAME
 *      ReleaseMutex - Releases the mutex.
 * 
 *  SYNOPSIS
 *      int ReleaseMutex(struct Mutex * mutex);
 * 
 *  FUNCTION
 *      Releases the mutex and eventually wakes other threads waiting for it.
 * 
 *  INPUTS
 *      mutex - The locked mutex.
 * 
 *  RESULT
 *      TRUE  - if the mutex is successfuly released.
 *      FALSE - in case of a problem with releasing the mutex.
 * 
 *  SEE ALSO
 *      CreateMutex(), DeleteMutex(), AttemptMutex(), ObtainMutex()
 */
int ReleaseMutex(struct Mutex *m)
{
    D(bug("[EXEC] ReleaseMutex(%p)\n", m));

    if (m != NULL && m->m_Node.ln_Type == NT_MUTEX)
    {
        int err;

        /* Release the mutex right now. */
        if (__sync_bool_compare_and_swap(&m->m_Lock, MUTEX_LOCKED, MUTEX_FREE))
        {
            /* If mutex is free send Wake signal to one waiting for the same mutex */
            err = syscall(SYS_futex, &m->m_Lock, FUTEX_WAKE, 1, NULL, NULL, 0);
            if (err < 0)
            {
                bug("[EXEC] Error releasing futex with err=%d\n", -err);
                return FALSE;
            }
        }
    }
    else
    {
        /* Not a mutex? Boo! Crash */
        bug("[EXEC] Not a mutex!\n");
        return FALSE;
    }

    return TRUE;
}
