#ifndef __ITF_00010060_H__
#define __ITF_00010060_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u8 idx;
    f32 dece;
} Itf_00010060_content_t;

typedef struct _Itf_00010060_t{
    Itf_00010060_content_t content;
    void (*packet_parse)(struct _Itf_00010060_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010060_t* , bus_packet *, u8 error_code);
} Itf_00010060_t;

Itf_00010060_t* Itf_00010060_new(void);

#endif

