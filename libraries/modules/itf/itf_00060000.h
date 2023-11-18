#ifndef __ITF_00060000_H__
#define __ITF_00060000_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 channel_id;
} Itf_00060000_content_t;

typedef struct _Itf_00060000_t {
    Itf_00060000_content_t content;
    void (*packet_parse)(struct _Itf_00060000_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_00060000_t*, bus_packet *, f32 temperature);
} Itf_00060000_t;

Itf_00060000_t *itf_00060000_new(void);

#endif
