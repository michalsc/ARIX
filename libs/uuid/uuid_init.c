#include <exec/types.h>
#include <exec/libraries.h>
#include <clib/uuid_protos.h>

#include <stdio.h>

extern struct Library *UUIDBase;
static void *_handle = NULL;

struct Library *Open(void *h, uint32_t version)
{
    (void)version;
    _handle = h;
    return UUIDBase;
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
