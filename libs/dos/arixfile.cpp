/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include "arixfile.h"
#include <proto/exec.h>
#include <uuid/uuid.h>
#include <exec/nodes.h>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <fcntl.h>

#define DEBUG
#include "dos_debug.h"
#include "dos_private.h"
#include "path.h"

int ARIXFile::Close()
{
    int err = -EBADF;

    if (__fd > 0)
    {
        err = syscall(SYS_close, __fd);
        __fd = 0;
    }

    return err;
}

size_t ARIXFile::Read(void * buffer, size_t length)
{
    int err = syscall(SYS_read, __fd, buffer, length);

    return err;
}

size_t ARIXFile::Write(const void * buffer, size_t length)
{
    int err = syscall(SYS_write, __fd, buffer, length);

    return err;
}

ARIXFile::ARIXFile(const char * p, int mode)
{
    __path = Path::PathFromDOS(p);

    D(bug("[DOS] ARIXFile::ARIXFile(%s, %d)\n", p, mode));
    D(bug("[DOS] Volume=%s, Path=%s\n", __path.volume(), __path.path()));

    #if 0
    int fd_vol;
    std::string path(p);
    std::string volume;

    printf("[DOS] ARIXFile::ARIXFile(%s, %d)\n", p, mode);

    // Try to find out requested volume and path within volume.
    std::size_t pos = path.find(":");
    // There is semicolon in path. if it is at position 0, search for current dir
    // and use it's own volume
    if (pos == 0)
    {
        ARIXFile *f = NULL;

        auto it = __files->find(__pr_CurrentDir);
        if (it != __files->end()) {
            f = it->second;
        }

        if (f) {
            volume = f->__volume;
            path = path.substr(1);
        }
    }
    // If there is semicolon in path and everything *before* it does not contain slash, then this is the volume
    else if (pos != std::string::npos && path.substr(0, pos).find_first_of("/\\|*#?") == std::string::npos)
    {
        volume = str_toupper(path.substr(0, pos));
        path = path.substr(pos+1);
    }
    else
    {
        // No semicolon was given in the name. Get the volume of current dir instead
        ARIXFile *f = NULL;
        auto it = __files->find(__pr_CurrentDir);
        if (it != __files->end()) {
            f = it->second;
        }

        if (f) {
            std::string _p = f->__path;
            volume = f->__volume;

            if (*(_p.rbegin()) != '/')
                _p.push_back('/');

            path = _p + path;
        }
    }

    if (path[0] == '/')
        path = path.substr(1);

    if (path == "")
        path = ".";

    printf("[DOS] Volume=%s\n", volume.c_str());
    printf("[DOS] Path=%s\n", path.c_str());

    // Simplify path and check if it goes out of volume/assign bounds.
    std::vector<std::string> path_array;
    bool success = true;
    while (path != "")
    {
        std::size_t pos = path.find('/');

        if (pos != std::string::npos) {
            std::string elem = path.substr(0, pos);
            if (elem != ".")
            {
                if (elem == "" || elem == "..")
                {
                    if (path_array.size() > 0)
                        path_array.pop_back();
                    else
                    {
                        success = false;
                        break;
                    }
                }
                else
                    path_array.push_back(elem);
            }
            path = path.substr(pos + 1);
        } else {
            if (path == "")
                continue;
            else if (path == "..") {
                if (path_array.size() > 0)
                    path_array.pop_back();
                else {
                    success = false;
                }
            }
            else
                path_array.push_back(path);
            path = "";
        }
    }

    if (!success) {
        printf("Path excaping the volume boundary");
    }
    else {
        path = "";
        for (int i=0; i < path_array.size(); i++)
        {
            if (i==0)
                path += path_array[i];
            else
                path += "/" + path_array[i];
            printf("element %3d: '%s'\n", i, path_array[i].c_str());
        }

        printf("[DOS] real path: '%s'\n", path.c_str());
    }

    // Try to open the volume first
    if (volume == "PROGDIR") {
        __fd = syscall(SYS_openat, __pr_HomeDirLock, path.c_str(), O_RDONLY);
    } else {
        fd_vol = syscall(SYS_openat, __AssignsDirLock, volume.c_str(), O_RDONLY | O_DIRECTORY);
        printf("fd_vol = %d\n", fd_vol);
        if (fd_vol > 0) {
            char buffer[32];
            int i=0;
            // Iterate through assigns as long as file opens or assigns are exhausted
            do {
                snprintf(buffer, 31, "%d", i);
                int fd_num = syscall(SYS_openat, fd_vol, buffer, O_RDONLY | O_DIRECTORY);
                printf("fd_num = %d for num %d\n", fd_num, i);
                if (fd_num < 0)
                    break;

                __fd = syscall(SYS_openat, fd_num, path.c_str(), mode);
                if (__fd > 0) {
                    syscall(SYS_close, fd_num);
                    break;
                }

                i++;
            } while(1);
            syscall(SYS_close, fd_vol);
        }
    }

    printf("__fd = %d\n", __fd);

    // Generate random uuid for the file
    int maxtry = 1000;

    do {
        __id = GetRandomID(NT_FILE);
    } while(!__files->empty() && (__files->count(__id) != 0 && --maxtry > 0));

    if (maxtry == 0) {
        printf("[DOS:ARIXFile] Error obtaining unique id\n");
    }

    std::ostringstream o;
    o << "[DOS] ARIXFile::ARIXFile(): id=" << __id << " after " << 1000 - maxtry << " attempts" << std::endl;;
    printf(o.str().c_str());

    (*__files)[__id] = this;
    #endif
}
