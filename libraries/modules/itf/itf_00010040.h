#ifndef __ITF_00010040_H__
#define __ITF_00010040_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u8 idx;
    f32 acce;
    f32 speed;
} Itf_00010040_content_t;

typedef struct _Itf_00010040_t{
    Itf_00010040_content_t content;
    void (*packet_parse)(struct _Itf_00010040_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010040_t* , bus_packet *, u8 error_code);
} Itf_00010040_t;

Itf_00010040_t* Itf_00010040_new(void);

#endif

