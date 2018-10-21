/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <exec/mutex.h>
#include <exec/memory.h>
#include <clib/exec_protos.h>

#include "exec_intern.h"

/**
 *  NAME
 *      CreateMutex - Allocate and initialize a mutex
 * 
 *  SYNOPSIS
 *      struct Mutex * CreateMutex(int initialState);
 * 
 *  FUNCTION
 *      Allocates memory for the mutex and sets its initial state to given
 *      value. You *must* delete the mutex by calling DeleteMutex()!
 * 
 *  INPUTS
 *      initialState - Initial state of the mutex. It can be one ot two
 *              values, either MUTEX_LOCKED or MUTEX_FREE.
 * 
 *  RESULT
 *      Mutex - Initialized mutex which can be used to guarantee exclusive 
 *              access across several threads of the process.
 * 
 *  SEE ALSO
 *      DeleteMutex(), ObtainMutex(), ReleaseMutex(), AttemptMutex()
 */
struct Mutex *CreateMutex(int initialState)
{
    bug("[EXEC] CreateMutex()\n");

    if (initialState != MUTEX_LOCKED)
    {
        initialState = MUTEX_FREE;
    }

    struct Mutex *m = AllocVec(sizeof(struct Mutex), MEMF_CLEAR);

    if (m != NULL)
    {
        m->m_Node.ln_Type = NT_MUTEX;
        m->m_Lock = initialState;
    }

    return m;
}
