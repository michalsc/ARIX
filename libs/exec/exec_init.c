/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <exec/types.h>
#include <exec/libraries.h>
#include <clib/exec_protos.h>
#include <sys/socket.h>

#include "tlsf.h"
#include "exec_intern.h"

#include <stdio.h>

extern struct Library * ExecBase;
static void * _handle = NULL;
void * local_memory_pool = NULL;
int OutSocket = 0;

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

void __attribute__((constructor)) ExecInit()
{
    printf("[EXEC] ExecInit()\n");
    local_memory_pool = tlsf_init_autogrow(65536, 0, NULL, NULL, NULL);
    printf("[EXEC] Local memory pool @ %p\n", local_memory_pool);
    OutSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
}

void __attribute__((destructor)) ExecDestroy()
{
    printf("[EXEC] ExecDestroy()\n");
    close(OutSocket);
    tlsf_destroy(local_memory_pool);
}
