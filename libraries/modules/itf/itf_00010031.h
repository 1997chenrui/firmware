#ifndef __ITF_00010031_H__
#define __ITF_00010031_H__
#include "macro.h"
#include "bus.h"

typedef enum {
    ITF_00010031_ABS = 0,
    ITF_00010031_REL
}Itf_00010031_cmd_e;

typedef struct {
    union
    {
        struct{
            u8 idx:4;
            u8 event_id:3;
            u8 is_rel:1;
        }data_u;
        u8 data;
    }idx_rel;
    f32 acce;
    f32 speed;
    f32 distance;
} Itf_00010031_content_t;

typedef struct _Itf_00010031_t{
    Itf_00010031_content_t content;
    void (*packet_parse)(struct _Itf_00010031_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010031_t* , bus_packet *, u8 error_code);
} Itf_00010031_t;

Itf_00010031_t* Itf_00010031_new(void);

#endif

