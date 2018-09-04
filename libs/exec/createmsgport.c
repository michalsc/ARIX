/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

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

static uint8_t getRand();
static uuid_t getRandomID();

/**
 * NAME
 *      CreateMsgPort - Allocate and initialize a message port
 * 
 * SYNOPSIS
 *      struct MsgPort * CreateMsgPort(void);
 * 
 * FUNCTION
 *      Allocates memory for the message port and creates local memory
 *      pool for message reception. Finally, random MsgPort ID is created
 *      either using kernel's random number generator or pseudo R.N.G.
 *      fallback. Finally, unix socket for incoming messages is bound to 
 *      the ID.
 * 
 *      You *must* delete the message port by calling DeleteMsgPort()!
 * 
 * RESULT
 *      MsgPort - Message port ready to receive incoming messages or NULL
 *              if either there was insufficient amount of memory or 
 *              underlying unix socket could not be created or binding to
 *              the ID failed.
 * 
 * SEE ALSO
 *      DeleteMsgPort(), AddPort(), RemPort()
*/
struct MsgPort * CreateMsgPort()
{
    struct sockaddr_un name;
    uuid_t *sock_id = (uuid_t *)&name.sun_path[1];
    struct MsgPort * port = AllocMem(sizeof(struct MsgPort), MEMF_CLEAR);

    printf("[EXEC] CreateMsgPort()\n");

    if (port)
    {
        // Create socket
        port->mp_Socket = syscall(SYS_socket, AF_UNIX, SOCK_DGRAM, 0);

        // If socket failed return NULL.
        if (port->mp_Socket < 0)
        {
            printf("[EXEC] Cannot create socket\n");
            FreeMem(port, sizeof(struct MsgPort));
            return NULL;
        }

        // Put the socket in non-blocking mode.
        int flags = syscall(SYS_fcntl, port->mp_Socket, F_GETFL);
        syscall(SYS_fcntl, port->mp_Socket, F_SETFL, flags | O_NONBLOCK);

        // Create memory pool for incomming messages.
        port->mp_MsgPool = CreatePool(0, 8192, 8192);
        if (port->mp_MsgPool == NULL)
        {
            printf("[EXEC] Cannot create memory pool\n");
            syscall(SYS_close, port->mp_Socket);
            FreeMem(port, sizeof(struct MsgPort));
            return NULL;
        }

        // Allocate first receive buffer of 4K size *in advance*
        port->mp_ReceiveBuffer = AllocVecPooled(port->mp_MsgPool, 4096);

        // Attempt to create random ID for port and bind socket to that ID
        name.sun_family = AF_UNIX;
        name.sun_path[0] = 0;
        int maxtry = 1000;      // Even with bad random generator 1000 retries shoudl be enough
        int err = 0;

        do {
            // Get ID and copy it to the name
            port->mp_ID = getRandomID();
            *sock_id = port->mp_ID;
            
            // Number of retries exhausted? Just fail...
            if (--maxtry < 0)
            {
                printf("[EXEC] too many tries. giving up\n");
                DeletePool(port->mp_MsgPool);
                syscall(SYS_close, port->mp_Socket);
                FreeMem(port, sizeof(struct MsgPort));
                return NULL;
            }
            
            printf("[EXEC] Binding to port ID {%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x}\n",
                   port->mp_ID.time_low, port->mp_ID.time_med, port->mp_ID.time_hi_and_version,
                   port->mp_ID.clock_seq_hi_and_reserved << 8 | port->mp_ID.clock_seq_low,
                   port->mp_ID.node[0], port->mp_ID.node[1], port->mp_ID.node[2],
                   port->mp_ID.node[3], port->mp_ID.node[4], port->mp_ID.node[5]);
            
            // Try to bind socket to the ID in unix socket namespace.
            err = syscall(SYS_bind, port->mp_Socket, (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(uuid_t));
        } while (err != 0);
    }

    return port;
}

/* 
    Fallback pseudo random number generator. UUID_Seed will be set
    by exec.library once the library loads
*/
uint32_t UUID_Seed = 0;

static uint8_t getRand()
{
    UUID_Seed = ((UUID_Seed * 1103515245) + 12345) & 0x7fffffff;
    return UUID_Seed & 0xff;
}

/*
    Get "unique" uuid by reading from random number generator. 
    Since the uuid is used for a message port, set the NT_MSGPORT
    node type in clock_seq_low.
*/
static uuid_t getRandomID()
{
    union {
        uuid_t uuid;
        uint8_t data[16];
    } id;

    /* 
        Using /dev/random would give better randomness, but it could be slow if there's not
        enough entropy. If opening /dev/urandom fails for some reason, fall back to 
        pseudo random number generator.
    */
    int fd = syscall(SYS_open, "/dev/urandom", O_RDONLY);
    if (fd > 0) {
        syscall(SYS_read, fd, id.data, 16);
        syscall(SYS_close, fd);
    } else {
        for (int i=0; i < 16; i++) {
            id.data[i] = getRand();
        }
    }

    /* Set the UUID type and version */
    id.uuid.clock_seq_hi_and_reserved &= 0x3f;
    id.uuid.clock_seq_hi_and_reserved |= 0x80;
    id.uuid.time_hi_and_version &= 0x0fff;
    id.uuid.time_hi_and_version |= 0x4000;
    id.uuid.clock_seq_low = NT_MSGPORT;

    return id.uuid;
}
