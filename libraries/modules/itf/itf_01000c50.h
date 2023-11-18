#ifndef __ITF_01000c50_H__
#define __ITF_01000c50_H__
#include "macro.h"
#include "bus.h"

typedef struct {
	u8 motor_id;
    f32 m1_k;
    f32 m2_b;
} Itf_01000c50_content_t;

typedef struct _Itf_01000c50_t{
    Itf_01000c50_content_t content;
    void (*packet_parse)(struct _Itf_01000c50_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000c50_t* , bus_packet *, u8 error_code);
} Itf_01000c50_t;

Itf_01000c50_t* Itf_01000c50_new(void);

#endif

