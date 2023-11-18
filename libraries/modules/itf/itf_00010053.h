#ifndef __ITF_00010053_H__
#define __ITF_00010053_H__
#include "macro.h"
#include "bus.h"

typedef enum{
    ITF_00010053_FORWARD = 0,
    ITF_00010053_REVERSE
}Itf_00010053_cmd_e;

typedef struct {
    union
    {
        struct{
            u8 idx:7;
            u8 is_forward:1;
        }data_u;
        u8 data;
    }idx_dir;
    f32 acce;
    f32 speed;
	u16 time_ms;
} Itf_00010053_content_t;

typedef struct _Itf_00010053_t{
    Itf_00010053_content_t content;
    void (*packet_parse)(struct _Itf_00010053_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010053_t* , bus_packet *, u8 error_code);
} Itf_00010053_t;

Itf_00010053_t* Itf_00010053_new(void);

#endif

