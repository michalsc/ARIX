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
}
