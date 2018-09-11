#include <uuid/uuid.h>
#include <clib/dos_protos.h>
#include <strings.h>
#include <string>

#include "dos_private.h"

#include <unordered_map>

std::unordered_map<uuid_t, std::string> assigns;

uuid_t Open(const char * name, int mode)
{
    std::string strname;
    /* Handle special case: path names starting with PROGDIR: */
    if (strncasecmp("PROGDIR:", name, 8) == 0)
    {
        int idx = 8;

        // Remove any / and . from the beginning of the path.
        while(name[idx] == '/' || name[idx] == '.')
            idx++;

        strname = __pr_HomePath;
        strname += &name[idx];

        printf("[DOS] special name: %s\n", strname.c_str());
    }
    (void)mode;
    return MAKE_UUID(0, 0, 0, 0, 0);
}
