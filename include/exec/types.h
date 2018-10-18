#ifndef _EXEC_TYPES_H
#define _EXEC_TYPES_H

#include <stdint.h>

typedef void        VOID;
typedef void *      APTR;
typedef uint8_t     UBYTE;
typedef int8_t      BYTE;
typedef uint16_t    UWORD;
typedef int16_t     WORD;
typedef uint32_t    ULONG;
typedef int32_t     LONG;
typedef uint64_t    UQUAD;
typedef int64_t     QUAD;
typedef uintptr_t   IPTR;
typedef intptr_t    SIPTR;

typedef int8_t      BOOL;

#ifndef NULL
#define NULL    ((APTR)0)
#endif // NULL

#define TRUE        1
#define FALSE       0

#endif // _EXEC_TYPES_H