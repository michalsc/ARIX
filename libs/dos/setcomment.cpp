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
#include <clib/dos_protos.h>
#include <uuid/uuid.h>
#include <fcntl.h>
#include <string.h>

#define DEBUG
#include "dos_debug.h"
#include "dos_private.h"
#include "arixfile.h"
#include "path.h"

int SetComment(const char *fileName, const char *comment)
{
    int err = -ENOENT;
    int commLength = strlen(comment);
    ARIXFile f(fileName);

    D(bug("[DOS] SetComment('%s', '%s')\n", fileName, comment));
    D(bug("[DOS] Volume=%s, Path=%s\n", f.path().volume().c_str(), f.path().path().c_str()));

    err = syscall(SYS_fsetxattr, f.fd(), "user.comment", comment, commLength, 0);

    D(bug("[DOS] SetComment returns %d\n", err));

    SetIoErr(-err);

    return err == 0 ? TRUE : FALSE;
}
