#include <exec/types.h>
#include <exec/libraries.h>

struct Library * OpenLibrary(const char * name, uint32_t minVersion)
{
    struct Library *retval = NULL;

    return retval;
}

struct Library * OldOpenLibrary(const char * name)
{
    return OpenLibrary(name, 0);
}
