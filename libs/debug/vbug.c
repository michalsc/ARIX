#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

void VBug(const char * format, va_list args)
{
    vprintf(format, args);
}
