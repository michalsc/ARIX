#ifndef __DEBUG_H
#define __DEBUG_H

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

#endif /* __DEBUG_H */
