#include <exec/types.h>
#include <exec/libraries.h>
#include <dlfcn.h>
#include <clib/exec_protos.h>

// Full path resolving not implemented yet
// Idea: use separate namespace for all arix libraries?
struct Library * OpenLibrary(const char * name, uint32_t minVersion)
{
    struct Library *retval = NULL;
    void *handle = NULL;

    // Try to open the library in lazy + local mode. Do not expose any symbols
    handle = dlopen(name, RTLD_LAZY | RTLD_LOCAL);

    if (handle) {
        // If the library was opened, search for the weak symbol LibBase in it.
        // The symbol points to library base with ID string and version number
        struct Library **sym = dlsym(handle, "LibBase");

        if (sym) {
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

        if (base == NULL) {
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
