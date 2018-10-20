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
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/libraries.h>
#include <clib/dos_protos.h>
#include <proto/exec.h>

#include <sys/socket.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "arixfile.h"
#include "../../system/arix/arix_messages.h"

extern struct Library *DOSBase;
static void *_handle = NULL;

extern "C" {
struct Library *LibOpen(void *h, uint32_t version);
void *LibClose();
void *LibExpunge();
void *GetHandle();
}

struct Library *LibOpen(void *h, uint32_t version)
{
    (void)version;
    _handle = h;
    return DOSBase;
}

void *LibClose()
{
    return _handle;
}

void *LibExpunge()
{
    return NULL;
}

void *GetHandle()
{
    return _handle;
}

intptr_t __pr_result_2;
char *__pr_HomePath;
int __pr_HomeDirLock;
int __AssignsDirLock;
int __TmpDirLock;
struct MsgPort * __pr_MsgPort;
std::unordered_map<uuid_t, ARIXFile *> *__files;
uuid_t __pr_CurrentDir;
struct Library * UtilityBase;

void __attribute__((constructor)) DOSInit()
{
    printf("[DOS] DOSInit()\n");
    uuid_t arixPort = MAKE_UUID(0x00000001, 0x0000, 0x4000, 0x8000 | NT_MSGPORT, 0x000000000000);

    if (!TestPort(arixPort))
    {
        printf("[DOS] ARIX Message Port not available. Consider booting ARIX first.\n");
        syscall(SYS_exit, 1);
    }

    UtilityBase = OpenLibrary("utility.library", 0);

    __pr_MsgPort = CreateMsgPort();
    if (__pr_MsgPort) {
        MsgARIXGetTempPath *m = (MsgARIXGetTempPath *)AllocVec(512, MEMF_CLEAR);
        if (m)
        {
            // Get the ARIX temp path (there are assigns volumes etc inside)
            // The path is sent back in form of ancillary data (file descriptor)
            // to the directory
            m->hdr.ma_Request = MSG_ARIX_GET_TEMP_PATH;
            m->hdr.ma_Message.mn_ReplyPort = __pr_MsgPort->mp_ID;
            m->hdr.ma_Message.mn_Length = 512 - sizeof(Message);

            PutMsg(arixPort, (Message *)m);
            FreeVec(m);

            WaitPort(__pr_MsgPort);
            m = (MsgARIXGetTempPath *)GetMsg(__pr_MsgPort);

            if (m->hdr.ma_Message.mn_Control != NULL)
            {
                struct msghdr msghdr;
                struct cmsghdr *cmsg;
                msghdr.msg_control = m->hdr.ma_Message.mn_Control;
                msghdr.msg_controllen = m->hdr.ma_Message.mn_ControlLength;

                printf("[DOS] Received control data at %p of size %ld\n", msghdr.msg_control, msghdr.msg_controllen);
                for (cmsg = CMSG_FIRSTHDR(&msghdr); cmsg; cmsg = CMSG_NXTHDR(&msghdr, cmsg))
                {
                    printf("[DOS] level: %d, type: %d, len: %d\n", cmsg->cmsg_level, cmsg->cmsg_type, cmsg->cmsg_len);
                    if (cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS) {
                        int fd = *(int*)CMSG_DATA(cmsg);
                        __TmpDirLock = fd;
                    }
                }
            }

            DiscardMsg((Message *)m);
        }
    }

    /* Initialize files */
    __files = new std::unordered_map<uuid_t, ARIXFile *>(std::size_t(512));

    /* Get path to the file */
    __pr_HomePath = static_cast<char*>(AllocVec(PATH_MAX, MEMF_CLEAR));
    size_t path_length = syscall(SYS_readlinkat, AT_FDCWD, "/proc/self/exe", __pr_HomePath, PATH_MAX);

    /* Remove executable name from the path */
    while(path_length > 0 && __pr_HomePath[path_length] != '/')
    {
        __pr_HomePath[path_length--] = 0;
    }

    /* Release as much memory as possible */
    __pr_HomePath = static_cast<char *>(ReallocMem(__pr_HomePath, path_length + 1));

    printf("[DOS] HomePath = %s\n", __pr_HomePath);

    __pr_HomeDirLock = syscall(SYS_openat, AT_FDCWD, __pr_HomePath, O_RDONLY | O_DIRECTORY);

    printf("[DOS] PROGDIR: lock = %d\n", __pr_HomeDirLock);

    __AssignsDirLock = syscall(SYS_openat, __TmpDirLock, ".assigns", O_RDONLY | O_DIRECTORY);
/*
    ARIXFile *f = new ARIXFile("sys:test_file.txt", O_RDONLY);
    f = new ARIXFile("progdir:a", O_RDONLY);
    new ARIXFile("SYS:../../file", O_RDONLY);
    new ARIXFile("SYS:", O_RDONLY | O_DIRECTORY);
    new ARIXFile("SYS:../ARIX", O_RDONLY | O_DIRECTORY);
    new ARIXFile("UNIX:/usr/local/bin/a", O_RDONLY);
    new ARIXFile("UNIX:/usr/local/./../bin/a", O_RDONLY);
    new ARIXFile("UNIX:/usr/local///bin/a", O_RDONLY);
*/
}

void __attribute__((destructor)) DOSDestroy()
{
    printf("[DOS] DOS says goodbye!\n");
    DeleteMsgPort(__pr_MsgPort);
}
