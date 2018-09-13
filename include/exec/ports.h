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
    struct MsgPort * mn_Owner;
    void *      mn_Control;
    uint16_t    mn_ControlLength;
    /* Starting from here the message is pushed through unix socket */
    uuid_t      mn_ReplyPort;
    uint8_t     mn_Type;
    uint8_t     mn_Pad;
    uint16_t    mn_Length;
};

#define MESSAGE_MAX_LENGTH      4096
#define MESSAGE_CTRL_MAX_LENGTH 512

#endif /* EXEC_PORTS_H */
