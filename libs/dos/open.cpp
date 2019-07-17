/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <uuid/uuid.h>
#include <clib/dos_protos.h>
#include <strings.h>
#include <string>

#include "dos_private.h"

uuid_t Open(const char * name, int mode)
{
    std::string strname;
    /* Handle special case: path names starting with PROGDIR: */
    if (strncasecmp("PROGDIR:", name, 8) == 0)
    {
        int idx = 8;

        // Remove any / and . from the beginning of the path.
        while(name[idx] == '/' || name[idx] == '.')
            idx++;

        strname = __pr_HomePath;
        strname += &name[idx];

        printf("[DOS] special name: %s\n", strname.c_str());
    }
    (void)mode;
    return MAKE_UUID(0, 0, 0, 0, 0);

    #if 0
        // Generate random uuid for the file
        int maxtry = 1000;

        do {
            __id = GetRandomID(NT_FILE);
        } while(!__files->empty() && (__files->count(__id) != 0 && --maxtry > 0));

        if (maxtry == 0) {
            bug("[DOS:ARIXFile] Error obtaining unique id\n");
            __err = -ENOMEM;
        }

        std::ostringstream o;
        o << "[DOS] ARIXFile::ARIXFile(): id=" << __id << " after " << 1000 - maxtry << " attempts" << std::endl;;
        D(bug(o.str().c_str()));

        (*__files)[__id] = this;
    #endif
}
