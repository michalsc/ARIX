#include <exec/types.h>
#include <exec/libraries.h>
#include <clib/exec_protos.h>

#include "tlsf.h"
#include "exec_intern.h"

#include <stdio.h>

extern struct Library * ExecBase;
static void * _handle = NULL;
void * local_memory_pool = NULL;

struct Library * Open(void * h, uint32_t version)
{
    _handle = h;
    return ExecBase;
}

void * Close()
{
    // Never return the .so handle. you shall not close exec.library!
    return NULL;
}

void * Expunge()
{
    return NULL;
}

void * GetHandle()
{
    return NULL;
}

void __attribute__((constructor)) ExecInit()
{
    printf("[EXEC] ExecInit()\n");
    local_memory_pool = tlsf_init_autogrow(65536, 0, NULL, NULL, NULL);
    printf("[EXEC] Local memory pool @ %p\n", local_memory_pool);
}

void __attribute__((destructor)) ExecDestroy()
{
    printf("[EXEC] ExecDestroy()\n");
    tlsf_destroy(local_memory_pool);
}
