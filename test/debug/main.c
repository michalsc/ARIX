#include <exec/libraries.h>
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/debug.h>

#include <stdio.h>

struct Library * DebugBase = NULL;

int main(int argc, char **argv)
{
    // Use c lib's printf. We are allowed to do that.
    printf("debug.library test\n");
    printf("SysBase = %p\n", ExecBase);

    DebugBase = OpenLibrary("debug.library", 0);

    printf("After attempting to open debug.library\nDebugBase = %p\n", DebugBase);

    Bug("Hello, world!\n");
}
