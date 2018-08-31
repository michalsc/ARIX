
#include <exec/types.h>
#include <exec/ports.h>
#include <exec/memory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <clib/exec_protos.h>
#include <fcntl.h>
#include <stdio.h>

#include "exec_intern.h"

static uuid_t getRandomID()
{
    union {
        uuid_t uuid;
        uint8_t data[16];
    } id;

    printf("getRandomID()\n");
    int fd = open("/dev/random", O_RDONLY);
    printf("fd=%d\n", fd);
    read(fd, id.data, 16);
    close(fd);
    printf("closed\n");
    id.uuid.clock_seq_hi_and_reserved &= 0x0f;
    id.uuid.clock_seq_hi_and_reserved |= 0x80;
    id.uuid.time_hi_and_version &= 0x0fff;
    id.uuid.time_hi_and_version |= 0x4000;

    return id.uuid;
}

struct MsgPort * CreateMsgPort()
{
    struct sockaddr_un name;
    struct MsgPort * port = AllocMem(sizeof(struct MsgPort), MEMF_CLEAR);
    printf("[EXEC] CreateMsgPort()\n");

    if (port) {
        printf("[EXEC] port=%p\n", port);
        port->mp_Socket = socket(AF_UNIX, SOCK_DGRAM, 0);

        if (port->mp_Socket < 0)
        {
            printf("[EXEC] Cannot create socket\n");
            FreeMem(port, sizeof(struct MsgPort));
            return NULL;
        }

        port->mp_MsgPool = CreatePool(0, 8192, 8192);
        if (port->mp_MsgPool == NULL)
        {
            printf("[EXEC] Cannot create memory pool\n");
            close(port->mp_Socket);
            FreeMem(port, sizeof(struct MsgPort));
            return NULL;
        }

        name.sun_family = AF_UNIX;
        name.sun_path[0] = 0;
        int maxtry = 1000;
        int err = 0;

        printf("[EXEC] Time for port id...\n");
        do {
            printf("maxtry=%d, err=%d\n", maxtry, err);
            port->mp_ID = getRandomID();
            printf("[EXEC] got random\n");
            CopyMem(&port->mp_ID, &name.sun_path[1], sizeof(uuid_t));
            printf("[EXEC] Copied\n");
            if (--maxtry < 0)
            {
                printf("[EXEC] too many tries. giving up\n");
                DeletePool(port->mp_MsgPool);
                close(port->mp_Socket);
                FreeMem(port, sizeof(struct MsgPort));
                return NULL;
            }
            printf("[EXEC] Trying to bind random port = {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
                   port->mp_ID.time_low, port->mp_ID.time_med, port->mp_ID.time_hi_and_version,
                   port->mp_ID.clock_seq_hi_and_reserved << 8 | port->mp_ID.clock_seq_low,
                   port->mp_ID.node[0], port->mp_ID.node[1], port->mp_ID.node[2],
                   port->mp_ID.node[3], port->mp_ID.node[4], port->mp_ID.node[5]);
        } while ((err = bind(port->mp_Socket, (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(uuid_t))));
    }

    return port;
}
