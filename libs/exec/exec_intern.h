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
#include <exec/mutex.h>
#include <uuid/uuid.h>
#include <time.h>
#include <utility/tagitem.h>

#ifdef __cplusplus

#include <list>

extern std::list<struct MsgPort *> __ports;

#endif

extern void * local_memory_pool;
extern struct Mutex local_memory_lock;
extern struct Mutex thread_sync_lock;
extern int OutSocket;
extern uint32_t UUID_Seed;
extern struct timespec StartTime;

struct PrivPool {
    void *      handle;
    struct Mutex mutex;
    uint32_t    requirements;
};

#ifdef __cplusplus
extern "C" {
#endif

struct TagItem *LibNextTagItem(struct TagItem **tagListPtr);
struct TagItem *LibFindTagItem(Tag tagValue, const struct TagItem *tagList);
IPTR LibGetTagData(Tag tagValue, IPTR defaultVal, const struct TagItem *tagList);
void bug(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _EXEC_INTERN_H */
