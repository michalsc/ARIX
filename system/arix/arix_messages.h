#ifndef _SYSTEM_ARIX_MESSAGES_H
#define _SYSTEM_ARIX_MESSAGES_H

#include <exec/ports.h>

struct MsgARIX {
    struct Message  ma_Message;
    uint32_t        ma_Request;
    intptr_t        ma_RetVal;
};

struct MsgARIXAddPort {
    struct MsgARIX  hdr;
    uuid_t          port;
    char            name[];
};

struct MsgARIXFindPort {
    struct MsgARIX  hdr;
    uuid_t          port;
    char            name[];
};

struct MsgARIXRemPort {
    struct MsgARIX  hdr;
    uuid_t          port;
};

struct MsgARIXGetUniqueID {
    struct MsgARIX  hdr;
    uint8_t         type;
    uuid_t          id;
};

struct MsgARIXGetTempPath {
    struct MsgARIX  hdr;
    char            path[];
};

#define MSG_ARIX_GET_UNIQUE_ID  0xCAFE0001
#define MSG_ARIX_ADD_PORT       0xCAFE0002
#define MSG_ARIX_REM_PORT       0xCAFE0003
#define MSG_ARIX_FIND_PORT      0xCAFE0004
#define MSG_ARIX_GET_TEMP_PATH  0xCAFE0005


#endif /* _SYSTEM_ARIX_MESSAGES_H */
