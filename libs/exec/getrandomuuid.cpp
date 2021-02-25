/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <unistd.h>

#include <clib/exec_protos.h>
#include <fcntl.h>
#include <stdio.h>

#include <proto/kernel.h>

#include "exec_intern.h"
#include "exec_debug.h"
#include "exec_random.h"

/*
    Get "unique" uuid by reading from random number generator. 
    Since the uuid is used for a message port, set the NT_MSGPORT
    node type in clock_seq_low.
*/
uuid_t GetRandomUUID(uint8_t type)
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
    int fd = SC_open("/dev/random", O_RDONLY);
    if (fd > 0) {
        SC_read(fd, id.data, 16);
        SC_close(fd);
    } else {
        for (int i=0; i < 16; i++) {
            id.data[i] = RandomNumberGenerator::get();
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
