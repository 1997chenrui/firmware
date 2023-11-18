#ifndef __ITF_00010082_H__
#define __ITF_00010082_H__
#include "macro.h"
#include "bus.h"

typedef enum{
    ITF_00010082_ABS = 0,
    ITF_00010082_REL
}Itf_00010082_cmd_e;

typedef struct {
    union
    {
        struct{
            u8 idx:7;
            u8 is_rel:1;
        }data_u;
        u8 data;
    }idx_rel;
    f32 acce;
    f32 speed;
    f32 distance;
} Itf_00010082_arg_t;

typedef struct {
    Itf_00010082_arg_t* args;
    u8 arg_len;
    u16 delay_ms;
} Itf_00010082_content_t;

typedef struct _Itf_00010082_t{
    Itf_00010082_content_t* content_array;
    u8 content_len;
    void (*packet_parse)(struct _Itf_00010082_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010082_t* , bus_packet *, u8 error_code);
} Itf_00010082_t;

Itf_00010082_t* Itf_00010082_new(void);

#endif

