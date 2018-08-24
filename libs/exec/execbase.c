#include <exec/types.h>
#include <exec/libraries.h>
#define NO_INLINES
#include <proto/exec.h>

static const char __idString[] = "\0$VER: exec.library 60.0 (24.08.2018)";

static const struct ExecBaseLVO __lvo = {

};

static const struct Library _ExecBase = {
    (const APTR)&__lvo, 
    60,
    0,
    (const APTR)&__idString[7],
};

const struct Library * LibBase __attribute__((weak)) = &_ExecBase;
const struct Library * ExecBase = &_ExecBase;
