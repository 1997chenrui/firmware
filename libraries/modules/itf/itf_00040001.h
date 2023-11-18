#ifndef __ITF_00040001_H__
#define __ITF_00040001_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 sensor_id;
} Itf_00040001_content_t;

typedef struct _Itf_00040001_t {
    Itf_00040001_content_t content;
    void (*packet_parse)(struct _Itf_00040001_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_00040001_t*, bus_packet *, u32 value);
} Itf_00040001_t;

Itf_00040001_t *itf_00040001_new(void);

#endif
