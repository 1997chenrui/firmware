#ifndef __ITF_00040010_H__
#define __ITF_00040010_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 sensor_id;
} Itf_00040010_content_t;

typedef struct _Itf_00040010_t {
    Itf_00040010_content_t content;
    void (*packet_parse)(struct _Itf_00040010_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_00040010_t*, bus_packet *, u32 value);
} Itf_00040010_t;

Itf_00040010_t *itf_00040010_new(void);

#endif
