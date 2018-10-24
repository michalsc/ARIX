/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _ARIXFILE_H
#define _ARIXFILE_H

#include <uuid/uuid.h>
#include <unordered_map>

#include "path.h"

enum EntryType {
    Volume, Assign
};

struct ARIXEntry {
    std::string     ae_Name;
    EntryType       ae_Type;
};

struct ARIXAssign : public ARIXEntry {
    std::string     aa_UnixPath;
};

class ARIXFile {
    Path    __path;
    uuid_t  __id;
    int     __fd;
public:
    ARIXFile(const char * path, int mode);
    static std::string arix2unix(std::string path);

    Path    path() { return __path; }
    int     fd() { return __fd; }
    uuid_t  id() { return __id; }
};

extern std::unordered_map<uuid_t, ARIXFile *> *__files;

#endif /* _ARIXFILE_H */
