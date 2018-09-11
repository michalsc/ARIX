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

#include <fcntl.h>

#include "dos_private.h"

static std::string str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c); });
    return s;
}

// Converts ARIX path of any kind (relative, absolute, etc) to unix path
std::string ARIXFile::arix2unix(std::string arix_path)
{

}

ARIXFile::ARIXFile(const char * p, int mode)
{
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

            if (*(_p.rbegin()) == '/')
                _p.pop_back();
            
            path = _p + path;
        }
    }

    if (path == "")
        path = ".";

    printf("[DOS] Volume=%s\n", volume.c_str());
    printf("[DOS] Path=%s\n", path.c_str());

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
}
