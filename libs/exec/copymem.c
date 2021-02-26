/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <clib/exec_protos.h>
#include <string.h>

void CopyMem(const void * source, void * dest, size_t byteSize)
{
    if (dest < source) {
        const char *s = source;
        char *d = dest;
        while (byteSize--)
            *d++ = *s++;
    }
    else if (byteSize != 0)
    {
        const char *s = source + byteSize;
        char *d = dest + byteSize;
        while (byteSize--)
            *--d = *--s;
    }
}

void CopyMemQuick(const void * source, void * dest, size_t byteSize)
{
    CopyMem(source, dest, byteSize);
}
