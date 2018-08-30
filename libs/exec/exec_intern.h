#ifndef _EXEC_INTERN_H
#define _EXEC_INTERN_H

#include <exec/ports.h>
#include <uuid/uuid.h>

extern void * local_memory_pool;

struct PrivPool {
    void *      handle;
    uint32_t    requirements;
};

struct MsgARIX {
    struct Message  ma_Message;
    uint32_t        ma_Request;
};

struct MsgARIXGetUniqueID {
    struct MsgARIX  hdr;
    uint8_t         type;
    uuid_t          id;
};

#define MSG_ARIX_GET_UNIQUE_ID  0xCAFE0001

#endif /* _EXEC_INTERN_H */
