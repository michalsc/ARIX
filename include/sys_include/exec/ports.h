#ifndef EXEC_PORTS_H
#define EXEC_PORTS_H

#include <stdint.h>
#include <exec/id.h>
#include <exec/mutex.h>
#include <exec/lists.h>

struct MsgPort
{
    struct ID   mp_ID;
    int         mp_Socket;
    struct Mutex mp_Lock;
    struct List mp_MsgList;
    void *      mp_MsgPool;
    void *      mp_ReceiveBuffer;
};

struct Message
{
    struct Node mn_Node;
    struct MsgPort * mn_Owner;
    void *      mn_Control;
    uint16_t    mn_ControlLength;
    /* Starting from here the message is pushed through unix socket */
    struct ID   mn_ReplyPort;
    uint8_t     mn_Type;
    uint8_t     mn_Pad;
    uint16_t    mn_Length;
};

#define MESSAGE_MAX_LENGTH      4096
#define MESSAGE_CTRL_MAX_LENGTH 512

#endif /* EXEC_PORTS_H */
