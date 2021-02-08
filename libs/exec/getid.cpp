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
#include <time.h>

#include <proto/kernel.h>

#include "exec_intern.h"
#include "exec_debug.h"
#include "exec_random.h"

namespace {
    uint16_t seq[256];
}

void InitializeIDSeq()
{
    int fd = SC_open("/dev/urandom", O_RDONLY);
    if (fd > 0) {
        SC_read(fd, seq, sizeof(seq));
        SC_close(fd);
    } else {
        for (int i=0; i < 256; i++) {
            seq[i] = RandomNumberGenerator::get<uint16_t>();
        }
    }
}

ID GetID(uint8_t type)
{
    ID newID;
    timespec t;

    SC_clock_gettime(CLOCK_MONOTONIC, &t);
    newID.raw = (uint64_t)((t.tv_sec * 1000 + t.tv_nsec / 1000000)) << 24;
    newID.raw |= type << 16;
    newID.raw |= seq[type]++;

    bug("[EXEC] GetID(%02x) returns {%010llx-%02x-%04x}\n", type, newID.raw >> 24, (newID.raw >> 16) & 0xff, newID.raw & 0xffff);

    return newID;
}