#ifndef __ITF_01000c12_H__
#define __ITF_01000c12_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    f32 m1_speed;
    f32 m2_acce;
    f32 m3_target_distance;
} Itf_01000c12_content_t;

typedef struct _Itf_01000c12_t{
    Itf_01000c12_content_t content;
    void (*packet_parse)(struct _Itf_01000c12_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000c12_t* , bus_packet *, u16 *slide_position,u16 slide_cnt);
} Itf_01000c12_t;

Itf_01000c12_t* Itf_01000c12_new(void);

#endif

