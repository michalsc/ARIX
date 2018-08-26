#include <exec/types.h>
#include <exec/libraries.h>
#include <dlfcn.h>
#include <clib/exec_protos.h>

#include <stdio.h>

#define bug printf
#define D(x) x

void CloseLibrary(struct Library * library)
{
    // If library argument was not null attempt to call the close vector of the library.
    // The vector shall return a handle to underlying .so object (the one that was passed
    // to the library in the Open vector. If Close() returns non-null value, dlclose will
    // be called on the returned handle

    D(bug("[EXEC] CloseLibrary(%p)\n", library));

    if (library) {
        struct LibraryLVO * lvo = library->lib_LVOTable;
        void * handle = lvo->Close();

        D(bug("[EXEC] Close returned %p\n", handle));

        if (handle) {
            D(bug("[EXEC] Closing dl library\n"));
            dlclose(handle);
        }
    }
}