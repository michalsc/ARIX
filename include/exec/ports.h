#ifndef EXEC_PORTS_H
#define EXEC_PORTS_H

#include <stdint.h>
#include <uuid/uuid.h>

struct MsgPort
{
    uuid_t      mp_ID;
    int         mp_Socket;
    void *      mp_MsgPool;
};

struct Message
{
    uuid_t      mn_ReplyPort;
    uint16_t    mn_Length;
    uint8_t     mn_Data[0];
};

#endif /* EXEC_PORTS_H */
