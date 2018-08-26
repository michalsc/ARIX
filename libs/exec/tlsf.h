#ifndef _TLSF_H
#define _TLSF_H

#include <stdint.h>
#include <exec/types.h>

typedef APTR (*autogrow_get)(APTR, IPTR *);
typedef void (*autogrow_release)(APTR, APTR, IPTR);

void *tlsf_init();
void *tlsf_init_autogrow(IPTR puddle_size, ULONG requirements, autogrow_get grow_function, autogrow_release release_function, APTR autogrow_data);
void tlsf_destroy(void *handle);
void tlsf_add_memory_and_merge(void *handle, APTR memory, IPTR size);
void tlsf_add_memory(void *handle, void *memory, IPTR size);

void *tlsf_malloc(void *handle, IPTR size, ULONG *flags);
void *tlsf_malloc_aligned(void *handle, IPTR size, IPTR align, ULONG *flags);
void tlsf_freevec(void *handle, APTR ptr);
void tlsf_freemem(void *handle, APTR ptr, IPTR size);
void *tlsf_realloc(void *handle, APTR ptr, IPTR new_size);
void *tlsf_allocabs(void *handle, IPTR size, void *ptr);

IPTR tlsf_avail(void *tlsf, ULONG requirements);
int tlsf_in_bounds(void *tlsf, APTR begin, APTR end);

#endif /* _TLSF_H */
