#ifndef _PROTO_DEBUG_H
#define _PROTO_DEBUG_H

#include <exec/types.h>
#include <exec/libraries.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

struct DebugBaseLVO {
    void (*VBug)(const char * format, va_list args);
};

#ifndef NO_INLINES

extern struct Library *DebugBase;

void VBug(const char * format, va_list args)
{
    ((struct DebugBaseLVO *)DebugBase->lib_LVOTable)->VBug(format, args);
}

void Bug(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    ((struct DebugBaseLVO *)DebugBase->lib_LVOTable)->VBug(format, args);
    va_end(args);
}

#endif // NO_INLINES

#endif // _PROTO_DEBUG_H
