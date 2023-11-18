#ifndef __ITF_00010010_H__
#define __ITF_00010010_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u8 id;
} Itf_00010010_content_t;

typedef struct {
    u8 is_zero_trigger;
    f32 current_cordinate;
    f32 lost_step;
    u8 current_level;
    u8 hold_level;
    u16 subdivision;
} Itf_00010010_rtn_t;

typedef struct _Itf_00010010_t{
    Itf_00010010_content_t content;
    void (*packet_parse)(struct _Itf_00010010_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010010_t* , bus_packet *, Itf_00010010_rtn_t* rtn_obj);
} Itf_00010010_t;

Itf_00010010_t* Itf_00010010_new(void);

#endif

