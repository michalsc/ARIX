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

#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include <fcntl.h>
#include <dirent.h>

#include "dos_private.h"
#include "path.h"
#include "arixfile.h"

#define DEBUG
#include "dos_debug.h"

static std::string str_toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c); });
    return s;
}

static std::vector<std::string> split_to_vector(std::string s, std::size_t skip=0)
{
    std::vector<std::string> list;
    std::size_t pos = 0;
    std::size_t lastpos = skip;

    do
    {
        pos = s.find('/', lastpos);
        std::string elem = s.substr(lastpos, pos - lastpos);

        list.push_back(elem);

        lastpos = pos + 1;
    } while (pos != std::string::npos);

    return list;
}

static std::list<std::string> split_to_list(std::string s, std::size_t skip=0)
{
    std::list<std::string> list;
    std::size_t pos = 0;
    std::size_t lastpos = skip;

    do
    {
        pos = s.find('/', lastpos);
        std::string elem = s.substr(lastpos, pos - lastpos);

        list.push_back(elem);

        lastpos = pos + 1;
    } while (pos != std::string::npos);

    return list;
}

static void print(const std::vector<std::string> list)
{
    D(bug("print_vector\n"));
    for (int i=0; i < list.size(); i++)
    {
        D(bug("element %3d: '%s'\n", i, list[i].c_str()));
    }
}

static void print(const std::list<std::string> list)
{
    D(bug("print_vector\n"));
    int i=0;
    for (auto it: list)
    {
        D(bug("element %3d: '%s'\n", i++, it.c_str()));
    }
}

Path Path::PathFromDOS(const std::string & path)
{
    Path p;

    D(bug("[DOS] Path::PathFromDOS(%s)\n", path.c_str()));

    // Try to find out requested volume and path within volume.
    std::size_t pos = path.find(":");

    // There is semicolon in path. if it is at position 0, get current dir
    // and use it's volume. Then, the path is absolute path within this volume
    if (pos == 0)
    {
        ARIXFile *f = NULL;

        auto it = __files->find(__pr_CurrentDir);
        if (it != __files->end()) {
            f = it->second;
        }

        if (f) {
            p.__volume = f->path().volume();
            p.__path = path.substr(1);
        }
    }
    // If there is semicolon in path and everything *before* it does not contain slash, then this is the volume
    else if (pos != std::string::npos && path.substr(0, pos).find_first_of("/\\|*#?") == std::string::npos)
    {
        p.__volume = str_toupper(path.substr(0, pos));
        p.__path = path.substr(pos+1);
    }
    // No volume and no semicolon at the beginning. The path is relative from current directory
    else
    {
        ARIXFile *f = NULL;

        auto it = __files->find(__pr_CurrentDir);
        if (it != __files->end()) {
            f = it->second;
        }

        if (f) {
            std::string _p = f->path().path();
            p.__volume = f->path().volume();

            if (*(_p.rbegin()) != '/')
                _p.push_back('/');

            p.__path = _p + path;
        }
    }

    if (p.__path[0] == '/')
        p.__path = p.__path.substr(1);

    if (p.__path == "")
        p.__path = ".";

    D(bug("[DOS] Before canonicalization: Volume=%s, Path=%s\n", p.__volume.c_str(), p.__path.c_str()));

    /* Canonicalize path and check if it does not try to go outside the volume */
    std::list<std::string> path_array = split_to_list(p.__path);
    bool success = true;
    std::size_t lastpos = 0;
    int cnt = 10;

    for (auto it = path_array.begin(); it != path_array.end(); ++it)
    {
        if (*it == "" || *it == "..")
        {
            if (it == path_array.begin())
                it = path_array.erase(it);
            else
            {
                it = std::prev(it);
                it = path_array.erase(it);
                it = path_array.erase(it);
            }
        }
    }

    auto it = path_array.begin();
    p.__path = *it;
    for (++it; it != path_array.end(); ++it)
    {
        p.__path += "/" + *it;
    }

    D(bug("[DOS] canonicalized path: '%s'\n", p.__path.c_str()));

    return p;
}

Path Path::PathFromUNIX(const std::string & path)
{
    Path p;

    D(bug("[DOS] Path::PathFromUNIX(%s)\n", path.c_str()));

    /* If the path is not absolute, continue with DOS path resolution */
    if (path[0] != '/')
    {
        return PathFromDOS(path);
    }
    else
    {
        /* Dealing with absolute path. Check mounted Volumes if it could be any of them */
        char linkbuf[512];
        bool found_match = false;

        /* First of all canonicalize the path. First step is to split the path into separate elements*/
        std::list<std::string> path_array = split_to_list(path, 1);
        print(path_array);
        bool restart_last = false;
        bool restart_all = false;
        int symlink_limit = 40;

        for (auto it = path_array.begin(); it != path_array.end(); ++it)
        {
            bool erased = false;
            std::string tmp;

            if (restart_last)
            {
                --it;
                restart_last = false;
            }

            if (restart_all)
            {
                it = path_array.begin();
                restart_all = false;
            }

            do
            {
                erased = false;

                if (*it == ".")
                {
                    it = path_array.erase(it);
                    erased = true;
                }

                if (*it == "..")
                {
                    if (it == path_array.begin())
                    {
                        it = path_array.erase(it);
                    }
                    else
                    {
                        it = path_array.erase(--it);
                        it = path_array.erase(it);
                    }
                    erased = true;
                }
            } while(erased && it != path_array.end());

            /* Try to build path from beginning up to this place */
            tmp = "";
            for (auto it2 = path_array.begin(); it2 != it; ++it2)
            {
                tmp += "/" + *it2;
            }
            tmp += "/" + *it;

            D(bug("test path: %s\n", tmp.c_str()));
            int linksize = syscall(SYS_readlinkat, AT_FDCWD, tmp.c_str(), linkbuf, 511);

            if (linksize > 0)
            {
                linkbuf[linksize] = 0;
                D(bug("path was a symlink pointing to: %s\n", linkbuf));

                if (--symlink_limit <= 0)
                {
                    D(bug("limit of symbolic links exceeded. breaking\n"));
                    break;
                }

                if (linkbuf[0] == '/')
                {
                    D(bug("absolute symlink, replacing everything up to this entry with it.\n"));
                    std::list<std::string> sym = split_to_list(linkbuf, 1);
                    sym.insert(sym.end(), std::next(it), path_array.end());
                    path_array = sym;
                    restart_all = true;
                }
                else
                {
                    // Remove current item (the symlink), and put new vector from symlink value in its
                    // place
                    std::list<std::string> sym = split_to_list(linkbuf);
                    it = path_array.erase(it);
                    it = path_array.insert(it, sym.begin(), sym.end());
                    restart_last = true;
                }
            }
        }

        print(path_array);

        for (auto it: path_array)
            p.__path += "/" + it;

        /* Check volumes */
        int fd = syscall(SYS_openat, __TmpDirLock, ".volumes", O_RDONLY | O_DIRECTORY);
        D(bug("volumes fd = %d\n", fd));

        if (fd > 0)
        {
            int len = 0;
            int bestmatch = 0;

            do {
                struct dirent64 de, *d=&de;
                len = syscall(SYS_getdents64, fd, &de, sizeof(de));
                if (len == 0)
                    break;

                do {
                    D(bug("len: %d\nentry: %s\n", d->d_reclen, d->d_name));
                    if ((d->d_name[0] == '.' && d->d_name[1] == 0) ||
                        (d->d_name[0] == '.' && d->d_name[1] == '.' && d->d_name[2] == 0))
                    {
                        D(bug("skipping entry...\n"));
                    }
                    else
                    {
                        D(bug("checking volume '%s'\n", d->d_name));
                        int sz = syscall(SYS_readlinkat, fd, d->d_name, linkbuf, 511);
                        linkbuf[sz] = 0;
                        D(bug("link pointing to: %s\n", linkbuf));
                        std::string b(linkbuf);

                        if (*b.rbegin() != '/')
                            b.push_back('/');

                        if (b == p.__path.substr(0, b.length()))
                        {
                            if (bestmatch < b.length())
                            {
                                p.__volume = d->d_name;
                                bestmatch = b.length();
                                found_match = true;
                            }
                        }
                    }

                    len -= d->d_reclen;
                    d = (struct dirent64 *)((intptr_t)d + d->d_reclen);
                } while (len > 0);
            } while(true);

            if (found_match)
                p.__path = p.__path.substr(bestmatch);

            syscall(SYS_close, fd);
        }

        /* Worst guess ever, assume this is a path within UNIX: assign */
        if (!found_match)
        {
            p.__volume = "UNIX";
        }

        D(bug("[DOS] Volume: %s, Path: %s\n", p.__volume.c_str(), p.__path.c_str()));
    }

    return p;
}
