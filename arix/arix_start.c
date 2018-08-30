#define _GNU_SOURCE
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <exec/nodes.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <proto/exec.h>
#include <uuid/uuid.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "../libs/exec/exec_intern.h"

struct MsgPort ARIXPort = {
    MAKE_UUID(0x00000001, 0x0000, 0x4000, 0x8000, 0xffff00000000 | NT_MSGPORT),
    0,
    NULL
};

static const char __attribute__((used)) version[] = "\0$VER: ARIX " VERSION_STRING;

typedef struct { uint64_t v64[2]; } uint128_t;

static uint128_t PortID = { 0, 1 };

uint128_t getPortID() {
    uint128_t id = PortID;
    uint64_t lo = PortID.v64[1];
    uint64_t hi = PortID.v64[0];

    lo++;
    if (lo == 0) hi++;

    PortID.v64[1] = lo;
    PortID.v64[0] = hi;

    return id;
}

int is_init()
{
    pid_t pid = (pid_t)syscall(SYS_getpid);

    return pid == 1;
}

/* 
    This is the entry point entered when linux kernel loads the file and begins
    the code execution
*/
int main(int argc, char **argv)
{
    struct sockaddr_un serverSocketAddr;
    union {
        union {
            struct MsgARIX base;
            struct MsgARIXGetUniqueID GetUniqueMsg;
        } msg;
        uint8_t b[4096];
    } buffer;

    printf("[ARIX] %s\n", &version[7]);

    ARIXPort.mp_Socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    ARIXPort.mp_MsgPool = CreatePool(MEMF_CLEAR, 8192, 8192);

    if (ARIXPort.mp_Socket < 0) {
        printf("[ARIX] Cannot create server socket\n");
        return 1;
    }

    memset(&serverSocketAddr.sun_path[0], 0, sizeof(serverSocketAddr.sun_path));
    serverSocketAddr.sun_family = AF_UNIX;
    CopyMem(&ARIXPort.mp_ID, &serverSocketAddr.sun_path[1], sizeof(uuid_t));

    /* Bind the UNIX domain address to the created socket */
    if (bind(ARIXPort.mp_Socket, (struct sockaddr *)&serverSocketAddr, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(uuid_t)))
    {
        printf("[ARIX] Failed to create master MsgPort. Is ARIX already running?\n");
        return 1;
    }

    while(1) {
        /*
        int nread = read(serverSocket, &buffer, sizeof(buffer));
        
        switch (buffer.msg.base.ma_Request) {
            case MSG_ARIX_GET_UNIQUE_ID:
                printf("[ARIX] Requesting unique ID for type %02x\n",
                    buffer.msg.GetUniqueMsg.type);
                if (buffer.msg.GetUniqueMsg.type == NT_MSGPORT) {
                    uuid_t *uuid = &buffer.msg.GetUniqueMsg.id;
                    uint128_t id = getPortID();
                    uuid->time_low = id.v64[1] & 0xffffffff;
                    uuid->time_med = (id.v64[1] >> 32) & 0xffff;
                    uuid->time_hi_and_version = 0x4000 | ((id.v64[1] >> 48) & 0x0fff);
                    uuid->clock_seq_low = (id.v64[1] >> 60);
                    uuid->clock_seq_low |= (id.v64[1] << 4) & 0xf0;
                    uuid->clock_seq_hi_and_reserved = 0x80 | ((id.v64[1] >> 4) & 0x0f);
                }
                break;

            default:
                printf("[ARIX] Unknown message\n");
                break;
        }
        */
    }
}
