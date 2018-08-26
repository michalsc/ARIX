#include <exec/libraries.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/debug.h>

#include <stdio.h>

struct Library *DebugBase = NULL;

static inline void Bug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    VBug(format, args);
    va_end(args);
}

int main(int argc, char **argv)
{
    // Use c lib's printf. We are allowed to do that.
    printf("debug.library test\n");
    printf("SysBase = %p\n", ExecBase);

    void *buffer = AllocVec(100, MEMF_CLEAR);
    printf("AllocMem returned %p\n", buffer);

    void *buffer2 = AllocVecAligned(100, 4096, MEMF_CLEAR);
    printf("AllocMemAligned returned %p\n", buffer);

    FreeVec(buffer);
    FreeVec(buffer2);

    DebugBase = OpenLibrary("debug.library", 0);

    printf("After attempting to open debug.library\nDebugBase = %p\n", DebugBase);
    
    Bug("Hello, world!\n");

    CloseLibrary(DebugBase);
}
