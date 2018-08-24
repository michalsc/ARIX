#include <exec/types.h>
#include <exec/libraries.h>
#define NO_INLINES
#include <proto/exec.h>

static const char __idString[] = "\0$VER: exec.library 60.0 (24.08.2018)\0";

void * AllocMem(size_t byteSize, uint32_t requirements);
void FreeMem(void *ptr);
struct Library * OpenLibrary(const char * name, uint32_t minVersion);
struct Library * OldOpenLibrary(const char * name);

static const struct ExecBaseLVO __lvo = {
    AllocMem,
    FreeMem,
    NULL,   // AvailMem
    OldOpenLibrary,
    OpenLibrary
};

static const struct Library _ExecBase = {
    (const APTR)&__lvo, 
    60,
    0,
    (const APTR)&__idString[7],
};

const struct Library * LibBase __attribute__((weak)) = &_ExecBase;
const struct Library * ExecBase = &_ExecBase;
