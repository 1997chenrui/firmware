#ifndef __ITF_00060020_H__
#define __ITF_00060020_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 id;
    f32 target_temp;
} Itf_00060020_content_t;

typedef struct _Itf_00060020_t {
    Itf_00060020_content_t content;
    void (*packet_parse)(struct _Itf_00060020_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_00060020_t *, bus_packet *, u8 error_code);
} Itf_00060020_t;

Itf_00060020_t *itf_00060020_new(void);
#endif
