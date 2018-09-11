/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef _EXEC_INTERN_H
#define _EXEC_INTERN_H

#include <exec/ports.h>
#include <uuid/uuid.h>
#include <time.h>

extern void * local_memory_pool;
extern int OutSocket;
extern uint32_t UUID_Seed;
extern struct timespec StartTime;

struct PrivPool {
    void *      handle;
    uint32_t    requirements;
};


#endif /* _EXEC_INTERN_H */
