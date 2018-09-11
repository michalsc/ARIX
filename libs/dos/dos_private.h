#ifndef _DOS_PRIVATE_H
#define _DOS_PRIVATE_H

#include <stdint.h>
#include <uuid/uuid.h>

extern intptr_t __pr_result_2;
extern char *__pr_HomePath;
extern int __pr_HomeDirLock;
extern int __AssignsDirLock;
extern int __TmpDirLock;
extern uuid_t __pr_CurrentDir;

#endif /* _DOS_PRIVATE_H */
