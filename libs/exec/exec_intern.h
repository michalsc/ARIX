#ifndef _EXEC_INTERN_H
#define _EXEC_INTERN_H

extern void * local_memory_pool;

struct PrivPool {
    void *      handle;
    uint32_t    requirements;
};

#endif /* _EXEC_INTERN_H */
