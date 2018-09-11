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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/uio.h>

#include <uuid/uuid.h>
#include <clib/exec_protos.h>

/**
 * NAME
 *      TestPort - Verify existence of message port
 * 
 * SYNOPSIS
 *      int TestPort(uuid_t portID);
 * 
 * FUNCTION
 *      Attempts to connect to the message port indentified by given portID.
 *      No message is actually send to the port, only port existence is
 *      verified.
 * 
 * INPUTS
 *      portID - UUID of the message port to be verified
 * 
 * RESULT
 *      0 if the message port is not existing, non-zero otherwise.
 * 
 * SEE ALSO
 *      FindPort(), PutMsg()
 */
int TestPort(uuid_t portID)
{
    struct sockaddr_un name = { AF_UNIX, {0,} };
    uuid_t *u = (uuid_t *)&name.sun_path[1];
    *u = portID;
    int sock;
    int ret = -1;

    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock > 0)
    {
        ret = connect(sock, (struct sockaddr *)&name, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(uuid_t));
        close(sock);
    }

    return (ret == 0) ? 1:0;
}
