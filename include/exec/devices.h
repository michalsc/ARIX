#ifndef _EXEC_DEVICES_H
#define _EXEC_DEVICES_H

#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/ports.h>

struct Device {
    struct Library  dd_Library;
};

struct Unit {
    struct MsgPort  unit_MsgPort;
    UBYTE           unit_Flags;
    UWORD           unit_OpenCnt;
};

struct IORequest;

// Default functions which shall be implemented by every library!
struct DeviceLVO {
    struct Device *     (*LibOpen)(void * handle, uint32_t version);
    void *              (*LibClose)();
    void *              (*LibExpunge)();
    void *              (*GetHandle)();
    void                (*BeginIO)(struct IORequest * io);
    int                 (*AbortIO)(struct IORequest * io);
};

#endif // _EXEC_DEVICES_H
