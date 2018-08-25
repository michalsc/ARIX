#include <exec/types.h>
#include <exec/libraries.h>
#include <clib/exec_protos.h>

extern struct Library * ExecBase;
static void * _handle = NULL;

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

int32_t ExtFunc()
{
    return 0;
}
