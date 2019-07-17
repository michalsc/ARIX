/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef _DOS_PRIVATE_H
#define _DOS_PRIVATE_H

#include <stdint.h>
#include <uuid/uuid.h>
#include <exec/libraries.h>

extern intptr_t __pr_result_2;
extern char *__pr_HomePath;
extern int __pr_HomeDirLock;
extern int __AssignsDirLock;
extern int __VolumesDirLock;
extern int __TmpDirLock;
extern uuid_t __pr_CurrentDir;
extern struct Library * UtilityBase;

#endif /* _DOS_PRIVATE_H */
