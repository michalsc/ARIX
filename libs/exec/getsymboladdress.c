/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <exec/types.h>
#include <exec/libraries.h>
#include <dlfcn.h>
#include <clib/exec_protos.h>

#include <stdio.h>

#define bug printf
#define D(x) x

// Full path resolving not implemented yet
// Idea: use separate namespace for all arix libraries?
void * GetSymbolAddress(struct Library * lib, const char *name)
{
    void *symbol = NULL;
    void *handle = NULL;

    D(bug("[EXEC] GetSymbolAddress(%p, %s)\n", (void *)lib, name));

    // Get handle of the library
    handle = ((struct LibraryLVO *)lib->lib_LVOTable)->GetHandle(lib);

    if (handle)
    {
        // If the handle is available, attempt to get address of the symbol
        symbol = dlsym(handle, name);

        D(bug("[EXEC] symbol: %p\n", symbol));
    }

    return symbol;
}
