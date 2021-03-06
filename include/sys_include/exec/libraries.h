#ifndef _EXEC_LIBRARIES_H
#define _EXEC_LIBRARIES_H

#include <exec/types.h>

struct Library {
    const APTR  lib_LVOTable;
    UWORD       lib_Version;
    UWORD       lib_Revision;
    const char *lib_IdString;
};

// Default functions which shall be implemented by every library!
struct LibraryLVO {
    struct Library *    (*LibOpen)(void * handle, uint32_t version);
    void *              (*LibClose)();
    void *              (*LibExpunge)();
    void *              (*GetHandle)();
};

#endif // _EXEC_LIBRARIES_H
