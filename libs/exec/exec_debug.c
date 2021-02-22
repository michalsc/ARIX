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

#include <proto/kernel.h>
#include <proto/debug.h>

void bug(const char * format, ...)
{
    if (DebugBase != NULL)
    {
        va_list arg;
        va_start(arg, format);
        VBug(format, arg);
        va_end(arg);
    }
}