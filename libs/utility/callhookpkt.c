#include <utility/hooks.h>
#include <clib/utility_protos.h>

uintptr_t CallHookPkt(struct Hook *hook, void * object, void * paramPacket)
{
    if (hook != NULL && hook->h_Entry != NULL)
        return hook->h_Entry(hook, object, paramPacket);
    else
        return 0;
}