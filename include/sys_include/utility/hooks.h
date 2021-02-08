#ifndef UTILITY_HOOKS_H
#define UTILITY_HOOKS_H

#include <exec/nodes.h>

/* A callback hook */
struct Hook {
    struct MinNode  h_MinNode;
    uintptr_t     (*h_Entry)(struct Hook *, void * object, void * message);
    void *          h_SubEntry;
    void *          h_Data;
};

static inline __attribute__((always_inline)) uintptr_t CALLHOOKPKT(struct Hook *hook, void *object, void *message) {
    return hook->h_Entry(hook, object, message);
}

#endif /* UTILITY_HOOKS_H */
