#ifndef __SYSCALL_H
#define __SYSCALL_H

#include <errno.h>

#ifdef __aarch64__

extern int __loc_errno;

static inline long sc0(long nr)
{
    register long ret __asm__("x0");
    register long _nr __asm__("x8") = nr;

    __asm__ volatile("svc #0":"=r"(ret):"r"(_nr):"cc");

    if (ret < 0 && ret > -134) {
        __loc_errno = -ret;
        ret = -1;
    }

    return ret;
}

static inline long sc1(long nr, long a1)
{
    register long ret __asm__("x0");
    register long _a1 __asm__("x0") = a1;
    register long _nr __asm__("x8") = nr;

    __asm__ volatile("svc #0":"=r"(ret):"r"(_a1),"r"(_nr):"cc");

    if (ret < 0 && ret > -134) {
        __loc_errno = -ret;
        ret = -1;
    }

    return ret;
}

static inline void __attribute__((noreturn)) sc1nr(long nr, long a1)
{
    register long _a1 __asm__("x0") = a1;
    register long _nr __asm__("x8") = nr;

    __asm__ volatile("svc #0"::"r"(_a1),"r"(_nr):"cc");
    while(1);
}

static inline long sc2(long nr, long a1, long a2)
{
    register long ret __asm__("x0");
    register long _a1 __asm__("x0") = a1;
    register long _a2 __asm__("x1") = a2;
    register long _nr __asm__("x8") = nr;

    __asm__ volatile("svc #0":"=r"(ret):"r"(_a1),"r"(_a2),"r"(_nr):"cc");

    if (ret < 0 && ret > -134) {
        __loc_errno = -ret;
        ret = -1;
    }

    return ret;
}

static inline long sc3(long nr, long a1, long a2, long a3)
{
    register long ret __asm__("x0");
    register long _a1 __asm__("x0") = a1;
    register long _a2 __asm__("x1") = a2;
    register long _a3 __asm__("x2") = a3;
    register long _nr __asm__("x8") = nr;

    __asm__ volatile("svc #0":"=r"(ret):"r"(_a1),"r"(_a2),"r"(_a3),"r"(_nr):"cc");

    if (ret < 0 && ret > -134) {
        __loc_errno = -ret;
        ret = -1;
    }

    return ret;
}

static inline long sc4(long nr, long a1, long a2, long a3, long a4)
{
    register long ret __asm__("x0");
    register long _a1 __asm__("x0") = a1;
    register long _a2 __asm__("x1") = a2;
    register long _a3 __asm__("x2") = a3;
    register long _a4 __asm__("x3") = a4;
    register long _nr __asm__("x8") = nr;

    __asm__ volatile("svc #0":"=r"(ret):"r"(_a1),"r"(_a2),"r"(_a3),"r"(_a4),"r"(_nr):"cc");

    if (ret < 0 && ret > -134) {
        __loc_errno = -ret;
        ret = -1;
    }

    return ret;
}

static inline long sc5(long nr, long a1, long a2, long a3, long a4, long a5)
{
    register long ret __asm__("x0");
    register long _a1 __asm__("x0") = a1;
    register long _a2 __asm__("x1") = a2;
    register long _a3 __asm__("x2") = a3;
    register long _a4 __asm__("x3") = a4;
    register long _a5 __asm__("x4") = a5;
    register long _nr __asm__("x8") = nr;

    __asm__ volatile("svc #0":"=r"(ret):"r"(_a1),"r"(_a2),"r"(_a3),"r"(_a4),"r"(_a5),"r"(_nr):"cc");

    if (ret < 0 && ret > -134) {
        __loc_errno = -ret;
        ret = -1;
    }

    return ret;
}

static inline long sc6(long nr, long a1, long a2, long a3, long a4, long a5, long a6)
{
    register long ret __asm__("x0");
    register long _a1 __asm__("x0") = a1;
    register long _a2 __asm__("x1") = a2;
    register long _a3 __asm__("x2") = a3;
    register long _a4 __asm__("x3") = a4;
    register long _a5 __asm__("x4") = a5;
    register long _a6 __asm__("x5") = a6;
    register long _nr __asm__("x8") = nr;

    __asm__ volatile("svc #0":"=r"(ret):"r"(_a1),"r"(_a2),"r"(_a3),"r"(_a4),"r"(_a5),"r"(_a6),"r"(_nr):"cc");

    if (ret < 0 && ret > -134) {
        __loc_errno = -ret;
        ret = -1;
    }

    return ret;
}

#endif

#endif // __SYSCALL_H
