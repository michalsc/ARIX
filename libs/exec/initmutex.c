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
 *      InitMutex - Initializes memory region as mutex
 * 
 *  SYNOPSIS
 *      void InitMutex(struct Mutex * mutex, int initialState);
 * 
 *  FUNCTION
 *      Initializes portion of memory as mutex and sets its initial 
 *      state to given value. 
 * 
 *  INPUTS
 *      mutex        - Memory area to be initialized as mutex.
 *      initialState - Initial state of the mutex. It can be one ot two
 *              values, either MUTEX_LOCKED or MUTEX_FREE.
 * 
 *  SEE ALSO
 *      CreateMutex(), DeleteMutex()
 */
void InitMutex(struct Mutex *m, int initialState)
{
    bug("[EXEC] InitMutex(%p)\n", m);

    if (initialState != MUTEX_LOCKED)
    {
        initialState = MUTEX_FREE;
    }

    if (m != NULL)
    {
        m->m_Node.ln_Type = NT_MUTEX;
        m->m_Lock = initialState;
    }
}
