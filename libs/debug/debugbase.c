#include <exec/types.h>
#include <exec/libraries.h>
#define NO_INLINES
#include <proto/debug.h>
#include <stdarg.h>

static const char __idString[] = "\0$VER: debug.library 60.0 (24.08.2018)\0";

void VBug(const char * format, va_list args);

static const struct DebugBaseLVO __lvo = {
    VBug
};

static const struct Library _DebugBase = {
    (const APTR)&__lvo, 
    60,
    0,
    (const APTR)&__idString[7],
};

const struct Library * LibBase __attribute__((weak)) = &_DebugBase;
const struct Library * DebugBase = &_DebugBase;
