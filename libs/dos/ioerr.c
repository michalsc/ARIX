#include <stdint.h>
#include <clib/dos_protos.h>

#include "dos_private.h"

intptr_t IoErr()
{
    intptr_t old_error = __pr_result_2;

    return old_error;
}
