#include <exec/types.h>
#include <exec/libraries.h>
#include <dlfcn.h>
#include <clib/exec_protos.h>

void CloseLibrary(struct Library * library)
{
    // If library argument was not null attempt to call the close vector of the library.
    // The vector shall return a handle to underlying .so object (the one that was passed
    // to the library in the Open vector. If Close() returns non-null value, dlclose will
    // be called on the returned handle
    
    if (library) {
        struct LibraryLVO * lvo = library->lib_LVOTable;
        void * handle = lvo->Close();

        if (handle) {
            dlclose(handle);
        }
    }
}