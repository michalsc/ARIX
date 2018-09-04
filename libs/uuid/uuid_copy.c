#include <exec/types.h>
#include <uuid/uuid.h>
#include <clib/uuid_protos.h>

void UUID_Copy(const uuid_t *src, uuid_t *dest)
{
    if (src != NULL && dest != NULL)
    {
        *dest = *src;
    }
}
