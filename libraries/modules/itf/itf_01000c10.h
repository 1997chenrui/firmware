#ifndef __ITF_01000c10_H__
#define __ITF_01000c10_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    f32 x_speed;
    f32 x_acce;
    f32 y_speed;
    f32 y_acce;
} Itf_01000c10_content_t;

typedef struct _Itf_01000c10_t{
    Itf_01000c10_content_t content;
    void (*packet_parse)(struct _Itf_01000c10_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000c10_t* , bus_packet *, u32);
} Itf_01000c10_t;

Itf_01000c10_t* Itf_01000c10_new(void);

#endif

