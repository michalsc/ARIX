#include <exec/types.h>
#include <exec/libraries.h>
#include <clib/utility_protos.h>

extern struct Library *UtilityBase;
static void *_handle = NULL;

struct Library *Open(void *h, uint32_t version)
{
    _handle = h;
    return UtilityBase;
}

void *Close()
{
    return _handle;
}

void *Expunge()
{
    return _handle;
}

void *GetHandle()
{
    return _handle;
}
