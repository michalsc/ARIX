/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <exec/libraries.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/debug.h>

#include <stdarg.h>
#include <stdio.h>

static const char __attribute__((used)) version[] = "\0$VER: DebugTest 60.0 " VERSION_STRING_DATE;

struct Library *DebugBase = NULL;

static inline void Bug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    VBug(format, args);
    va_end(args);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    // Use c lib's printf. We are allowed to do that.
    printf("debug.library test\n");
    printf("SysBase = %p\n", (void *)ExecBase);

    void *buffer = AllocVec(100, MEMF_CLEAR);
    printf("AllocMem returned %p\n", buffer);

    void *buffer3 = AllocVec(100, MEMF_CLEAR);
    printf("AllocMem returned %p\n", buffer3);

    void *buffer2 = AllocVecAligned(240000, 262144, MEMF_CLEAR);
    printf("AllocMemAligned returned %p\n", buffer2);

    FreeVec(buffer);
    FreeVec(buffer2);
    FreeVec(buffer3);

    DebugBase = OpenLibrary("debug.library", 0);

    printf("After attempting to open debug.library\nDebugBase = %p\n", (void *)DebugBase);
    
    Bug("Hello, world!\n");

    CloseLibrary(DebugBase);
}
