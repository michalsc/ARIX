/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

#include <clib/exec_protos.h>
#include <fcntl.h>
#include <stdio.h>

#include "exec_intern.h"

/* 
    Fallback pseudo random number generator. UUID_Seed will be set
    by exec.library once the library loads
*/
uint32_t UUID_Seed = 0;

static uint8_t getRand()
{
    UUID_Seed = ((UUID_Seed * 1103515245) + 12345) & 0x7fffffff;
    return UUID_Seed & 0xff;
}

/*
    Get "unique" uuid by reading from random number generator. 
    Since the uuid is used for a message port, set the NT_MSGPORT
    node type in clock_seq_low.
*/
uuid_t GetRandomID(uint8_t type)
{
    union {
        uuid_t uuid;
        uint8_t data[16];
    } id;

    /* 
        Using /dev/random would give better randomness, but it could be slow if there's not
        enough entropy. If opening /dev/urandom fails for some reason, fall back to 
        pseudo random number generator.
    */
    int fd = syscall(SYS_open, "/dev/urandom", O_RDONLY);
    if (fd > 0) {
        syscall(SYS_read, fd, id.data, 16);
        syscall(SYS_close, fd);
    } else {
        for (int i=0; i < 16; i++) {
            id.data[i] = getRand();
        }
    }

    /* Set the UUID type and version */
    id.uuid.clock_seq_hi_and_reserved &= 0x3f;
    id.uuid.clock_seq_hi_and_reserved |= 0x80;
    id.uuid.time_hi_and_version &= 0x0fff;
    id.uuid.time_hi_and_version |= 0x4000;
    id.uuid.clock_seq_low = type;

    return id.uuid;
}