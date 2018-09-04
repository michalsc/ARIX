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
struct Library * OpenLibrary(const char * name, uint32_t minVersion)
{
    struct Library *retval = NULL;
    void *handle = NULL;

    D(bug("[EXEC] OpenLibrary(%s, %x)\n", name, minVersion));

    // Try to open the library in lazy + local mode. Do not expose any symbols
    handle = dlopen(name, RTLD_LAZY | RTLD_LOCAL);

    D(bug("[EXEC] dlopen returned %p\n", handle));

    if (handle) {
        // If the library was opened, search for the weak symbol LibBase in it.
        // The symbol points to library base with ID string and version number
        struct Library **sym = dlsym(handle, "LibBase");

        D(bug("[EXEC] pointer to symbol: %p\n", (void *)sym));

        if (sym) {

            D(bug("[EXEC] LibBase = %p\n", (void *)*sym));

            retval = *sym;
            if (retval->lib_Revision < minVersion) {
                retval = NULL;
            }
        }
    }

    // If return value is set it means the library was successfully loaded and now
    // is in address space of caller. Call librarys Open vector passing dl handle
    // and version number. Give Library the ability to change library base and, if
    // it returned NULL assume there was a failure and issue dlclose immediately.
    if (retval) {
        struct LibraryLVO *lvo = retval->lib_LVOTable;
        struct Library * base = lvo->Open(handle, minVersion);

        D(bug("[EXEC] Open function returned %p\n", (void *)base));

        if (base == NULL) {
            D(bug("[EXEC] Open failed. Closing the library\n"));
            dlclose(handle);
        } else { retval = base; }
    }

    return retval;
}

// Old version of OpenLibrary, keept for nostalgic reasons. It ignores the version
// number and just opens the library
struct Library * OldOpenLibrary(const char * name)
{
    return OpenLibrary(name, 0);
}
