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
#include <exec/libraries.h>
#include <clib/exec_protos.h>
#include <sys/socket.h>
#include <time.h>

#include "tlsf.h"
#include "exec_intern.h"
#include "exec_debug.h"
#include "exec_random.h"

#include <proto/kernel.h>

#include <stdio.h>

#include <list>

std::list<struct MsgPort *> __ports;

extern struct Library * ExecBase;
static void * _handle = NULL;
void * local_memory_pool = NULL;
struct Mutex local_memory_lock;
struct Mutex thread_sync_lock;
int OutSocket = 0;
struct ID ARIXPort = ARIX_PORT_ID;

struct timespec StartTime;

#ifdef __cplusplus

extern "C" {
    struct Library *Open(void *h, uint32_t version);
    void *Close();
    void *Expunge();
    void *GetHandle();
}

#endif

struct Library * Open(void * h, uint32_t version)
{
    (void)version;
    _handle = h;
    return ExecBase;
}

void * Close()
{
    // Never return the .so handle. you shall not close exec.library!
    return NULL;
}

void * Expunge()
{
    return NULL;
}

void * GetHandle()
{
    return NULL;
}

void InitializeIDSeq();

void __attribute__((constructor)) ExecInit()
{
    bug("[EXEC] ExecInit()\n");
    local_memory_pool = tlsf_init_autogrow(65536, 0, NULL, NULL, NULL);
    bug("[EXEC] Local memory pool @ %p\n", local_memory_pool);
    InitMutex(&local_memory_lock, MUTEX_FREE);
    InitMutex(&thread_sync_lock, MUTEX_FREE);
    OutSocket = SC_socket(AF_UNIX, SOCK_DGRAM, 0);
    SC_clock_gettime(CLOCK_REALTIME, &StartTime);
    bug("[EXEC] Start time: %ld.%09ld\n", StartTime.tv_sec, StartTime.tv_nsec);
    RandomNumberGenerator::seed();
    InitializeIDSeq();
}

void __attribute__((destructor)) ExecDestroy()
{
    bug("[EXEC] ExecDestroy()\n");

    if (!__ports.empty()) {
        bug("[EXEC] Not all MsgPorts were closed. Closing now...\n");

        while (!__ports.empty()) {
            struct MsgPort *p = __ports.front();
            __ports.remove(p);

            DeleteMsgPort(p);
        }
    }

    SC_close(OutSocket);
    tlsf_destroy(local_memory_pool);
}

int __syscall_error(int err)
{
    return err;
}
