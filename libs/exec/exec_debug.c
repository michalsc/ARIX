#define _GNU_SOURCE 1
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void bug(const char * format, ...)
{
    char buffer[512];

    va_list arg;
    va_start(arg, format);
    vsnprintf(buffer, 511, format, arg);
    va_end(arg);
    buffer[511] = 0;

    syscall(SYS_write, 1, buffer, strlen(buffer));
}