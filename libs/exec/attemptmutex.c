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

/**
 *  NAME
 *      AttemptMutex - Tries to obtain the mutex without waiting
 * 
 *  SYNOPSIS
 *      int AttemptMutex(struct Mutex * mutex);
 * 
 *  FUNCTION
 *      Attempts to obtain the mutex. If the mutex was free and successfuly
 *      obtained by this function, TRUE is returned. If the lock failed and
 *      mutex is still in posession of another thread, FALSE is returned.
 * 
 *  INPUTS
 *      mutex - The allocated and initialized mutex to be locked.
 * 
 *  RESULT
 *      TRUE  - if the mutex is successfuly obtained,
 *      FALSE - otherwise
 * 
 *  SEE ALSO
 *      CreateMutex(), DeleteMutex(), ObtainMutex(), ReleaseMutex()
 */
int AttemptMutex(struct Mutex *m)
{
    bug("[EXEC] AttemptMutex(%p)\n", m);

    if (m != NULL && m->m_Node.ln_Type == NT_MUTEX)
    {
        /* 
            If mutex was free, set it to locked state and return TRUE,
            return FALSE otherwise.
        */
        if (__sync_bool_compare_and_swap(&m->m_Lock, MUTEX_FREE, MUTEX_LOCKED))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        /* Not a Mutex? Return FALSE, something GURU-like could be better */
        bug("[EXEC] Not a mutex!\n");
        return FALSE;
    }

    return TRUE;
}
