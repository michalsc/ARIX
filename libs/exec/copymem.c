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
    memmove(dest, source, byteSize);
}

void CopyMemQuick(const void * source, void * dest, size_t byteSize)
{
    memmove(dest, source, byteSize);
}