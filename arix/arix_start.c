#define GNU_SOURCE
#include <sys/syscall.h>

/* 
    This is the entry point entered when linux kernel loads the file and begins
    the code execution
*/
int _start()
{
    syscall(SYS_exit, 0);
}
