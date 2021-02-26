/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <linux/futex.h>
#include <errno.h>

#include <exec/mutex.h>
#include <clib/exec_protos.h>

#include <proto/kernel.h>

#include "exec_debug.h"
#include "exec_intern.h"

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

int internalReleaseMutex(uint32_t *lock, struct Library *KernelBase)
{
    if (__sync_bool_compare_and_swap(lock, MUTEX_LOCKED, MUTEX_FREE))
    {
        return SC_futex(lock, FUTEX_WAKE, 1, NULL, NULL, 0);
    }
    return 0;
}

int ReleaseMutex(struct Mutex *m)
{
    D(bug("[EXEC] ReleaseMutex(%p)\n", m));

    if (m != NULL && m->m_Node.ln_Type == NT_MUTEX)
    {
        int err;

        /* Release the mutex right now. */
        #if 0
        if (__sync_bool_compare_and_swap(&m->m_Lock, MUTEX_LOCKED, MUTEX_FREE))
        {
            /* If mutex is free send Wake signal to one waiting for the same mutex */
            err = SC_futex(&m->m_Lock, FUTEX_WAKE, 1, NULL, NULL, 0);
            if (err < 0)
            {
                bug("[EXEC] Error releasing futex with err=%d\n", -err);
                return FALSE;
            }
        }
        #else
        if ((err = internalReleaseMutex(&m->m_Lock, KernelBase)) < 0)
        {
            bug("[EXEC] Error releasing futex with err=%d\n", -err);
            return FALSE;
        }
        #endif
    }
    else
    {
        /* Not a mutex? Boo! Crash */
        bug("[EXEC] Not a mutex!\n");
        return FALSE;
    }

    return TRUE;
}
