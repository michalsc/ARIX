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
#include <exec/tasks.h>
#include <proto/exec.h>
#include <proto/kernel.h>
#include <proto/debug.h>
#include <uuid/uuid.h>

#include <linux/limits.h>
#include <sys/mount.h>
#include <stdarg.h>
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
void messageLoop(struct MsgPort *arix);

#define ARIX_TEMP_PATH "/tmp/.arix/"

struct MsgPort ARIXPort;

int dir_tmp;

static const char __attribute__((used)) version[] = "\0$VER: ARIX 60.0 " VERSION_STRING_DATE;

struct Library *DebugBase = NULL;

static inline void Bug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    VBug(format, args);
    va_end(args);
}

int is_init()
{
    pid_t pid = SC_getpid();

    return pid == 1;
}

/*
    This is the entry point entered when linux kernel loads the file and begins
    the code execution
*/
int main(int argc, char **argv)
{
    int dir_assigns;
    int dir_volumes;
    int dir_devices;

    char *sys_path;

    (void)argc;
    (void)argv;

    DebugBase = OpenLibrary("debug.library", 0);

    Bug("[ARIX] argc = %d\n", argc);
    for (int i=0; i < argc; i++)
    {
        Bug("[ARIX] argv[%d] = '%s'\n", i, argv[i]);
    }

    if (argc > 1)
    {
        char buff[256];
        int fd = atoi(argv[1]);
        int count = SC_read(fd, buff, 256);
        Bug("[ARIX] read from fd %d got %d bytes: '%s'\n", fd, count, buff);
        SC_close(fd);
    }

    struct sockaddr_un serverSocketAddr;
    union {
        union {
            struct MsgARIX base;
            struct MsgARIXGetUniqueID GetUniqueMsg;
        } msg;
        uint8_t b[4096];
    } buffer;

    (void)buffer;

    Bug("[ARIX] %s\n", &version[7]);

    ARIXPort.mp_ID = ARIX_PORT_ID;
    NEWLIST(&ARIXPort.mp_MsgList);
    ARIXPort.mp_Socket = SC_socket(AF_UNIX, SOCK_DGRAM, 0);
    ARIXPort.mp_MsgPool = CreatePool(MEMF_CLEAR, 8192, 8192);
    ARIXPort.mp_ReceiveBuffer = AllocVecPooled(ARIXPort.mp_MsgPool, 4096);
    InitMutex(&ARIXPort.mp_Lock, MUTEX_FREE);

    int flags = SC_fcntl(ARIXPort.mp_Socket, F_GETFL, 0);
    SC_fcntl(ARIXPort.mp_Socket, F_SETFL, flags | O_NONBLOCK);

    if (ARIXPort.mp_Socket < 0) {
        Bug("[ARIX] Cannot create server socket\n");
        return 1;
    }

    memset(&serverSocketAddr.sun_path[0], 0, sizeof(serverSocketAddr.sun_path));
    serverSocketAddr.sun_family = AF_UNIX;
    CopyMem(&ARIXPort.mp_ID, &serverSocketAddr.sun_path[1], sizeof(uuid_t));

    /* Bind the UNIX domain address to the created socket */
    if (SC_bind(ARIXPort.mp_Socket, (struct sockaddr *)&serverSocketAddr, offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(ID)))
    {
        Bug("[ARIX] Failed to create master MsgPort. Is ARIX already running?\n");
        return 1;
    }

    /* Prepare temporary directory for assigns and volumes */
    SC_mkdir(ARIX_TEMP_PATH, 0755);
    /* Unshare mount namespace */
    SC_unshare(CLONE_NEWNS);
    /* All mounts under /tmp are private and do not propagate outside this namespace */
    //SC_mount("none", "/tmp", NULL, MS_REC | MS_PRIVATE, NULL);
    /*
        Mount tmpfs there. The mount is invisible outside ARIX process, however ARIX can
        pass file descriptor from it to the outside world through messages
    */
    SC_mount("none", ARIX_TEMP_PATH, "tmpfs", 0, NULL);
    dir_tmp = SC_open(ARIX_TEMP_PATH, O_RDONLY | O_DIRECTORY);

    SC_mkdirat(dir_tmp, ".assigns", 0755);
    dir_assigns = SC_openat(dir_tmp, ".assigns", O_RDONLY | O_DIRECTORY);

    SC_mkdirat(dir_tmp, ".volumes", 0755);
    dir_volumes = SC_openat(dir_tmp, ".volumes", O_RDONLY | O_DIRECTORY);

    SC_mkdirat(dir_tmp, ".devices", 0755);
    dir_devices = SC_openat(dir_tmp, ".devices", O_RDONLY | O_DIRECTORY);

    /* Synthesize RAM: device (Ram Disk Volume) */
    SC_mkdirat(dir_devices, "RAM", 0755);
    SC_mount("none", ARIX_TEMP_PATH "/.devices/RAM", "tmpfs", 0, NULL);
    SC_symlinkat(ARIX_TEMP_PATH "/.devices/RAM/", dir_volumes, "Ram Disk");

    /* Synthesize UNIX: assign */
    SC_mkdirat(dir_assigns, "UNIX", 0755);
    SC_symlinkat("/", dir_assigns, "UNIX/0");

    /* Get path to the file */
    sys_path = static_cast<char*>(AllocVec(PATH_MAX, MEMF_CLEAR));
    size_t path_length = SC_readlink("/proc/self/exe", sys_path, PATH_MAX);

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
    SC_mkdirat(dir_assigns, "SYS", 0755);
    SC_symlinkat(sys_path, dir_assigns, "SYS/0");

    struct TagItem tags[] = {
        { TASKTAG_PC,           (IPTR)messageLoop },
        { TASKTAG_STACKSIZE,    262144 },
        { TASKTAG_NAME,         (IPTR)"ARIX Msg Loop" },
        { TASKTAG_ARG1,         (IPTR)&ARIXPort },
        { TAG_DONE, 0 }
    };

    CreateThread(tags);

    struct Library *base = OpenLibrary("dos.library", 0);

    Bug("[ARIX] DOSBase = %p\n", base);

    Bug("[ARIX] Idling here...\n");

    while(1) {
        struct timespec tv;
        tv.tv_sec = 1;
        tv.tv_nsec = 0;

        SC_clock_nanosleep(CLOCK_MONOTONIC, 0, &tv, NULL);
    }
}

void messageLoop(struct MsgPort * arix)
{
    Bug("[ARIX] Starting message loop on port %p\n", (void*)arix);

    while (1)
    {
        WaitPort(arix);
        struct MsgARIX *msg;
        int spincnt = 2000;
        while (--spincnt)
        {
            while ((msg = (struct MsgARIX *)GetMsg(arix)))
            {
                if (msg->ma_Message.mn_Length >= (sizeof(MsgARIX) - sizeof(Message)))
                {
                    switch (msg->ma_Request)
                    {
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
                        ID zero = NULL_ID;
                        if (m->port == zero)
                        {
                            m->hdr.ma_RetVal = 0;
                        }
                        else
                        {
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
                        *(int *)CMSG_DATA(&u.cmsg) = dir_tmp;
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
                        Bug("[ARIX] Unhandeld request %08x\n", msg->ma_Request);
                        break;
                    }
                }
                ReplyMsg((struct Message *)msg);
            }
        }
    }
}
