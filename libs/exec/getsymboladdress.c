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

    D(bug("[EXEC] GetSymbolAddress(%p, %s)\n", lib, name));

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
