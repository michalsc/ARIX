#ifndef UUID_UUID_H
#define UUID_UUID_H

#include <stdint.h>
#include <unistd.h>

typedef uint64_t uuid_time_t;

typedef struct 
{
    uint8_t nodeID[6];
} uuid_node_t;

typedef struct 
{
    uint32_t    time_low;
    uint16_t    time_med;
    uint16_t    time_hi_and_version;
    uint8_t     clock_seq_hi_and_reserved;
    uint8_t     clock_seq_low;
    uint8_t     node[6];
} uuid_t;

#define MAKE_UUID(a, b, c, d, e)           \
    {                                      \
        (a), (b), (c),                     \
            ((d) >> 8) & 0xFF, (d)&0xFF,   \
        {                                  \
            (uint64_t)(e) >> 40,              \
                ((uint64_t)(e) >> 32) & 0xFF, \
                ((e) >> 24) & 0xFF,        \
                ((e) >> 16) & 0xFF,        \
                ((e) >> 8) & 0xFF,         \
                (e)&0xFF                   \
        }                                  \
    }

typedef enum {
    UUID_NAMESPACE_DNS = 1,
    UUID_NAMESPACE_URL,
    UUID_NAMESPACE_OID,
    UUID_NAMESPACE_X500
} uuid_namespace_t;

typedef enum
{
    UUID_TYPE_DCE_TIME = 1,
    UUID_TYPE_DCE_RANDOM = 4
} uuid_type_t;

typedef enum
{
    UUID_VARIANT_NCS = 0,
    UUID_VARIANT_DCE = 1,
    UUID_VARIANT_MICROSOFT = 2,
    UUID_VARIANT_OTHER = 3
} uuid_variant_t;

#define UUID_STRLEN 36

#ifdef __cplusplus

static inline bool operator==(uuid_t first, uuid_t second) {
    union {
        uint64_t a[2];
        uuid_t u;
    } u1, u2;

    u1.u = first;
    u2.u = second;

    return (u1.a[0] == u2.a[0] && u1.a[1] == u2.a[1]);
}

static inline bool operator<(uuid_t first, uuid_t second) {
    union {
        uint64_t a[2];
        uuid_t u;
    } u1, u2;

    u1.u = first;
    u2.u = second;

    return (u1.a[0] < u2.a[0] || ((u1.a[0] == u2.a[0]) && (u1.a[1] < u2.a[1])));
}

static inline bool operator>(uuid_t first, uuid_t second) {
    union {
        uint64_t a[2];
        uuid_t u;
    } u1, u2;

    u1.u = first;
    u2.u = second;

    return (u1.a[0] > u2.a[0] || ((u1.a[0] == u2.a[0]) && (u1.a[1] > u2.a[1])));
}

// uuid specialization for std::hash 
#include <functional>
#include <ostream>
#include <stdio.h>
namespace std
{
    template <> struct hash<uuid_t>
    {
        size_t operator()(const uuid_t & id) const noexcept
        {
            union {
                uint64_t u64[2];
                uint32_t u32[4];
                uuid_t uuid;
            } u;
            u.uuid = id;
            if (sizeof(size_t) == 32) {
                return u.u32[0] ^ u.u32[1] ^ u.u32[2] ^ u.u32[3];
            } else {
                return u.u64[0] ^ u.u64[1];
            }
        }
    };

    static inline ostream& operator<<(ostream &strm, const uuid_t &id) {
        char tmpbuf[UUID_STRLEN + 1];
        snprintf(tmpbuf, UUID_STRLEN + 1, "%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x",
                id.time_low, id.time_med, id.time_hi_and_version, id.clock_seq_hi_and_reserved << 8 | id.clock_seq_low,
                id.node[0], id.node[1], id.node[2], id.node[3], id.node[4], id.node[5]);
        strm << (char *)tmpbuf;
        return strm;
    }
}


#endif

#endif /* UUID_UUID_H */
