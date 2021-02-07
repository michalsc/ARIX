#include <exec/devices.h>
#include <exec/io.h>
#include <clib/exec_protos.h>

int32_t AbortIO(struct IORequest *request)
{
    (void)request;
    return 0;
}