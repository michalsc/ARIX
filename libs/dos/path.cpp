/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <string>
#include <vector>
#include <algorithm>

#include "dos_private.h"
#include "path.h"
#include "arixfile.h"

#define DEBUG
#include "dos_debug.h"

static std::string str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c); });
    return s;
}

Path Path::PathFromDOS(std::string path)
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
    std::vector<std::string> path_array;
    bool success = true;
    while (p.__path != "")
    {
        std::size_t pos = p.__path.find('/');

        if (pos != std::string::npos) {
            std::string elem = p.__path.substr(0, pos);
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
            p.__path = p.__path.substr(pos + 1);
        } else {
            if (p.__path == "")
                continue;
            else if (p.__path == "..") {
                if (path_array.size() > 0)
                    path_array.pop_back();
                else {
                    success = false;
                }
            }
            else
                path_array.push_back(p.__path);
            p.__path = "";
        }
    }

    if (!success) {
        bug("Path excaping the volume boundary");
    }
    else {
        p.__path = path_array[0];
        for (int i=1; i < path_array.size(); i++)
        {
            p.__path += "/" + path_array[i];
            D(bug("element %3d: '%s'\n", i, path_array[i].c_str()));
        }

        D(bug("[DOS] canonicalized path: '%s'\n", p.__path.c_str()));
    }

    return p;
}

Path Path::PathFromUNIX(std::string path)
{
    Path p;

    return p;
}
