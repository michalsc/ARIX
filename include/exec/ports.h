#ifndef EXEC_PORTS_H
#define EXEC_PORTS_H

#include <stdint.h>
#include <uuid/uuid.h>

struct MsgPort
{
    uuid_t      mp_ID;
    int         mp_Socket;
    void *      mp_MsgPool;
    void *      mp_ReceiveBuffer;
};

struct Message
{
    struct MsgPort * mn_ReplyPort;
    struct MsgPort * mn_ReceivePort;
    uint8_t     mn_Type;
    uint8_t     mn_Pad;
    uint16_t    mn_Length;
    uint8_t     mn_Data[0];
};

#endif /* EXEC_PORTS_H */
