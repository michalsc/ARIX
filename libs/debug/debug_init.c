#include <exec/types.h>
#include <exec/libraries.h>
#include <clib/debug_protos.h>

#include <stdio.h>

extern struct Library *DebugBase;
static void *_handle = NULL;

struct Library *Open(void *h, uint32_t version)
{
    (void)version;
    _handle = h;
    return DebugBase;
}

void *Close()
{
    return _handle;
}

void *Expunge()
{
    return NULL;
}

void *GetHandle()
{
    return _handle;
}
