#ifndef __ITF_01000940_H__
#define __ITF_01000940_H__
#include "macro.h"
#include "bus.h"

typedef enum{
    ITF_01000940_ABS = 0,
    ITF_01000940_REL
}Itf_01000940_cmd_e;

typedef struct {
    union
    {
        struct{
            u8 idx:6;
            u8 event_id:1;
            u8 is_rel:1;
        }data_u;
        u8 data;
    }idx_rel;
    f32 acce;
    f32 speed;
    f32 distance;
} Itf_01000940_content_t;

typedef struct _Itf_01000940_t{
    Itf_01000940_content_t content;
    void (*packet_parse)(struct _Itf_01000940_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000940_t* , bus_packet *, u8 error_code, f32 trigger_distance);
} Itf_01000940_t;

Itf_01000940_t* Itf_01000940_new(void);

#endif

