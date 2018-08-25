#ifndef _EXEC_LIBRARIES_H
#define _EXEC_LIBRARIES_H

#include <exec/types.h>

struct Library {
    const APTR  lib_LVOTable;
    UWORD       lib_Version;
    UWORD       lib_Revision;
    const APTR  lib_IdString;
};

// Default functions which shall be implemented by every library!
struct LibraryLVO {
    struct Library *    (*Open)(void * handle, uint32_t version);
    void *              (*Close)();
    void *              (*Expunge)();
    int32_t             (*ExtFunc)();
};

#endif // _EXEC_LIBRARIES_H
