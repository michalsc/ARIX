#ifndef _EXEC_LIBRARIES_H
#define _EXEC_LIBRARIES_H

#include <exec/types.h>

struct Library {
    const APTR  lib_LVOTable;
    UWORD       lib_Version;
    UWORD       lib_Revision;
    const APTR  lib_IdString;
};

#endif // _EXEC_LIBRARIES_H