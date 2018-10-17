/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#undef _GNU_SOURCE
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

#include <linux/limits.h>
#include <sys/mount.h>
#include <string.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <map>

#include "arix_messages.h"

#include "publicport.h"

PublicPortList PublicPorts;

#define ARIX_TEMP_PATH "/tmp/.arix/"

struct MsgPort ARIXPort = {
    MAKE_UUID(0x00000001, 0x0000, 0x4000, 0x8000 | NT_MSGPORT, 0x000000000000),
    0,
    NULL, NULL};

static const char __attribute__((used)) version[] = "\0$VER: ARIX 60.0 " VERSION_STRING_DATE;

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
    int dir_tmp;
    int dir_assigns;
    char *sys_path;

    (void)argc;
    (void)argv;
    struct sockaddr_un serverSocketAddr;
    union {
        union {
            struct MsgARIX base;
            struct MsgARIXGetUniqueID GetUniqueMsg;
        } msg;
        uint8_t b[4096];
    } buffer;

    (void)buffer;

    std::cout << "[ARIX] " << &version[7] << std::endl;

    ARIXPort.mp_Socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    ARIXPort.mp_MsgPool = CreatePool(MEMF_CLEAR, 8192, 8192);
    ARIXPort.mp_ReceiveBuffer = AllocVecPooled(ARIXPort.mp_MsgPool, 4096);

    int flags = fcntl(ARIXPort.mp_Socket, F_GETFL);
    fcntl(ARIXPort.mp_Socket, F_SETFL, flags | O_NONBLOCK);

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

    /* Mount procfs */
    syscall(SYS_mount, "proc", "/proc", "proc", 0, NULL);

    /* Prepare temporary directory for assigns and volumes */
    syscall(SYS_mkdir, ARIX_TEMP_PATH, 0755);
    /* Unshare mount namespace */
    syscall(SYS_unshare, CLONE_NEWNS);
    /* All mounts under /tmp are private and do not propagate outside this namespace */
    syscall(SYS_mount, "none", "/tmp", NULL, MS_REC | MS_PRIVATE, NULL);
    /*
        Mount tmpfs there. The mount is invisible outside ARIX process, however ARIX can 
        pass file descriptor from it to the outside world through messages
    */
    syscall(SYS_mount, "none", ARIX_TEMP_PATH, "tmpfs", 0, NULL);
    dir_tmp = syscall(SYS_open, ARIX_TEMP_PATH, O_RDONLY | O_DIRECTORY);

    syscall(SYS_mkdirat, dir_tmp, ".assigns", 0755);
    dir_assigns = syscall(SYS_openat, dir_tmp, ".assigns", O_RDONLY | O_DIRECTORY);
    
    /* Synthesize UNIX: assign */
    syscall(SYS_mkdirat, dir_assigns, "UNIX", 0755);
    syscall(SYS_symlinkat, "/", dir_assigns, "UNIX/0");

    /* Get path to the file */
    sys_path = static_cast<char*>(AllocVec(PATH_MAX, MEMF_CLEAR));
    size_t path_length = syscall(SYS_readlink, "/proc/self/exe", sys_path, PATH_MAX);
std::cout << "path_length = " << path_length << ", sys_path=" << sys_path << std::endl;

    /* Remove executable name from the path */
    while(path_length > 0 && sys_path[path_length] != '/')
    {
        sys_path[path_length--] = 0;
    }

    /* Remove one directory level from the path, ARIX has to be in SYS:System/ directory */
    path_length--;
    while(path_length > 0 && sys_path[path_length] != '/')
    {
        sys_path[path_length--] = 0;
    }

    /* Release as much memory as possible */
    sys_path = static_cast<char *>(ReallocMem(sys_path, path_length + 1));

    /* Synthesize SYS: assign */
    syscall(SYS_mkdirat, dir_assigns, "SYS", 0755);
    syscall(SYS_symlinkat, sys_path, dir_assigns, "SYS/0");

    while(1) {
        WaitPort(&ARIXPort);
        struct MsgARIX *msg;
        int spincnt=2000;
        while(--spincnt)
        {
            while((msg = (struct MsgARIX *)GetMsg(&ARIXPort)))
            {
                if (msg->ma_Message.mn_Length >= (sizeof(MsgARIX) - sizeof(Message)))
                {
                    switch (msg->ma_Request) {
                        case MSG_ARIX_ADD_PORT:
                        {
                            struct MsgARIXAddPort *m = reinterpret_cast<struct MsgARIXAddPort *>(msg);
                            bool ret = PublicPorts.addPort(m->port, m->name);
                            m->hdr.ma_RetVal = ret;
                            break;
                        }
                        case MSG_ARIX_REM_PORT:
                        {
                            struct MsgARIXRemPort *m = reinterpret_cast<struct MsgARIXRemPort *>(msg);
                            PublicPorts.remPort(m->port);
                            m->hdr.ma_RetVal = 1;
                            break;
                        }
                        case MSG_ARIX_FIND_PORT:
                        {
                            struct MsgARIXFindPort *m = reinterpret_cast<struct MsgARIXFindPort *>(msg);
                            m->port = PublicPorts.findPort(m->name);
                            uuid_t zero = MAKE_UUID(0, 0, 0, 0, 0);
                            if (m->port == zero) {
                                m->hdr.ma_RetVal = 0;
                            }
                            else {
                                m->hdr.ma_RetVal = 1;
                            }
                            break;
                        }
                        case MSG_ARIX_GET_TEMP_PATH:
                        {
                            struct MsgARIXGetTempPath *m = reinterpret_cast<struct MsgARIXGetTempPath *>(msg);
                            union {
                                char buffer[CMSG_SPACE(sizeof(int))];
                                struct cmsghdr cmsg;
                            } u;

                            // Fill in the control data in order to send the socket
                            u.cmsg.cmsg_level = SOL_SOCKET;
                            u.cmsg.cmsg_type = SCM_RIGHTS;
                            u.cmsg.cmsg_len = CMSG_LEN(sizeof(int));
                            *(int*)CMSG_DATA(&u.cmsg) = dir_tmp;
                            m->hdr.ma_Message.mn_Control = u.buffer;
                            m->hdr.ma_Message.mn_ControlLength = sizeof(u.buffer);
                            m->hdr.ma_RetVal = 1;

                            // Reply the message here already, otherwise cmsg buffer runs out of scope and
                            // may be not valid when the main ReplyMsg is called
                            ReplyMsg((struct Message *)msg);
                            msg = NULL;
                            break;
                        }
                        default:
                            std::cout << "[ARIX] Unhandeld request " << msg->ma_Request << std::endl;
                            break;
                    }
                }
                ReplyMsg((struct Message *)msg);
            }
        }
//        printf("processed %d\n", n);
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
