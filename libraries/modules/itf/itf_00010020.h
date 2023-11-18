#ifndef __ITF_00010020_H__
#define __ITF_00010020_H__
#include "macro.h"
#include "bus.h"

typedef enum{
    ITF_00010020_NO_MODIFY = 0,
    ITF_00010020_MODIFY
}Itf_00010020_cmd_e;

typedef struct {
    u8 idx;
    f32 acce;
    f32 speed;
} Itf_00010020_content_t;

typedef struct _Itf_00010020_t{
    Itf_00010020_content_t content;
    void (*packet_parse)(struct _Itf_00010020_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010020_t* , bus_packet *, u8 error_code);
} Itf_00010020_t;

Itf_00010020_t* Itf_00010020_new(void);

#endif

