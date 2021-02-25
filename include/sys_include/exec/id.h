#ifndef _EXEC_ID_H
#define _EXEC_ID_H

#include <exec/types.h>
#include <exec/nodes.h>

struct ID {
    uint64_t raw;
};

#define MAKE_ID_T(time, type, seq) {((uint64_t)(time) << 24) | (((type) & 0xff) << 16) | ((seq) & 0xffff)}

/* ARIX port has a constant ID of 0000000001-04-0000 */
#define ARIX_PORT_ID    MAKE_ID_T(1, NT_MSGPORT, 0)
/* NULL ID is 0000000000-00-0000 */
#define NULL_ID         MAKE_ID_T(0, 0, 0)

static inline uint64_t GET_ID_TIME(struct ID id) { return id.raw >> 24; }
static inline uint8_t GET_ID_TYPE(struct ID id) { return (id.raw >> 16) & 0xff; }
static inline uint16_t GET_ID_SEQ(struct ID id) { return id.raw & 0xffff; }

#define ID_STRLEN 18

#ifdef __cplusplus

static inline bool operator==(ID first, ID second) {
    return (first.raw == second.raw);
}

static inline bool operator<(ID first, ID second) {
    return (first.raw < second.raw);
}

static inline bool operator>(ID first, ID second) {
    return (first.raw > second.raw);
}

// uuid specialization for std::hash 
#include <functional>
#include <ostream>
#include <stdio.h>
namespace std
{
    template <> struct hash<ID>
    {
        size_t operator()(const ID & id) const noexcept
        {
            union {
                uint64_t u64[1];
                uint32_t u32[2];
                ID  id;
            } u;
            u.id = id;
            if (sizeof(size_t) == 32) {
                return u.u32[0] ^ u.u32[1];
            } else {
                return u.u64[0];
            }
        }
    };

    static inline ostream& operator<<(ostream &strm, const ID &id) {
        char tmpbuf[ID_STRLEN + 1];
        snprintf(tmpbuf, ID_STRLEN + 1, "%010llx-%02x-%04x",
                id.raw >> 24, (id.raw >> 16) & 0xff, id.raw & 0xffff);
        strm << (char *)tmpbuf;
        return strm;
    }
}


#endif

#endif // _EXEC_ID_H
