#ifndef EXEC_MUTEX_H
#define EXEC_MUTEX_H

#include <stdint.h>
#include <exec/types.h>
#include <exec/nodes.h>

struct Mutex {
    struct Node m_Node;
    int32_t     m_Lock;
};

#define MUTEX_LOCKED    0
#define MUTEX_FREE      1

#ifdef __cplusplus

#include <exception>
#ifndef CLIB_EXEC_PROTOS_H
extern "C" {
int ObtainMutex(APTR);
int ReleaseMutex(APTR);
}
#endif

class MutexLocker {
    APTR m;
public:
    explicit MutexLocker(APTR mutex) { m = mutex; if (ObtainMutex(m) == FALSE) throw std::exception(); }
    ~MutexLocker() { ReleaseMutex(m); }
};

#endif

#endif /* EXEC_MUTEX_H */
