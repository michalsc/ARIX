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

#endif /* EXEC_MUTEX_H */
