#include <exec/types.h>
#include <uuid/uuid.h>
#include <clib/uuid_protos.h>

void UUID_Clear(uuid_t * uuid)
{
    if (uuid != NULL)
    {
        uuid->time_low = uuid->time_med = uuid->time_hi_and_version = 0;
        uuid->clock_seq_low = uuid->clock_seq_hi_and_reserved = 0;
        for (unsigned int i=0; i < sizeof(uuid->node); i++)
        {
            uuid->node[i] = 0;
        }
    }
}
