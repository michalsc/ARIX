/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <stdint.h>
#include <clib/dos_protos.h>

#include "dos_private.h"

intptr_t SetIoErr(intptr_t new_error)
{
    intptr_t old_error = __pr_result_2;
    
    __pr_result_2 = new_error;
    
    return old_error;
}
