#include <stdint.h>
#include <clib/dos_protos.h>

#include "dos_private.h"

intptr_t SetIoErr(intptr_t new_error)
{
    intptr_t old_error = __pr_result_2;
    
    __pr_result_2 = new_error;
    
    return old_error;
}
