#ifndef __ITF_000100a6_H__
#define __ITF_000100a6_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    f32 cal_distance;
} Itf_000100a6_content_t;

typedef struct _Itf_000100a6_t{
    Itf_000100a6_content_t content;
    void (*packet_parse)(struct _Itf_000100a6_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_000100a6_t* , bus_packet *,u8 errcode, u8 last,u8 current);
} Itf_000100a6_t;

Itf_000100a6_t* Itf_000100a6_new(void);

#endif


