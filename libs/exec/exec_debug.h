#ifndef _EXEC_DEBUG_H
#define _EXEC_DEBUG_H

#ifdef DEBUG
#define D(x) x
#else
#define D(x) /* */
#endif

#ifdef __cplusplus
extern "C" {
#endif

void bug(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _EXEC_DEBUG_H */
