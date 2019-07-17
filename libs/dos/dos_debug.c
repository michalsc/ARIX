/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#define _GNU_SOURCE 1
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void bug(const char * format, ...)
{
    char buffer[512];

    va_list arg;
    va_start(arg, format);
    vsnprintf(buffer, 511, format, arg);
    va_end(arg);
    buffer[511] = 0;

    syscall(SYS_write, 1, buffer, strlen(buffer));
}
