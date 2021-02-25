#ifndef _EXEC_IO_H
#define _EXEC_IO_H

#include <exec/ports.h>
#include <exec/devices.h>
#include <exec/id.h>
#include <stddef.h>

struct IORequest
{
    struct Message  io_Message;
    struct ID       io_ID;
    struct Device * io_Device;
    struct Unit *   io_Unit;
    UWORD           io_Command;
    UBYTE           io_Flags;
    LONG            io_Error;
};

struct IOStdReq
{
    struct IORequest io_Req;
    size_t          io_Actual;
    size_t          io_Length;
    void *          io_Data;
    size_t          io_Offset;
};

#define CMD_INVALID     0
#define CMD_RESET       1
#define CMD_READ        2
#define CMD_WRITE       3
#define CMD_UPDATE      4
#define CMD_CLEAR       5
#define CMD_STOP        6
#define CMD_START       7
#define CMD_FLUSH       8
#define CMD_NONSTD      9

#define IOB_QUICK   0
#define IOF_QUICK   (1 << IOB_QUICK)

#endif // _EXEC_IO_H
