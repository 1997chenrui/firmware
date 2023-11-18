#ifndef __ITF_00040020_H__
#define __ITF_00040020_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 sensor_id;
} Itf_00040020_content_t;

typedef struct _Itf_00040020_t {
    Itf_00040020_content_t content;
    void (*packet_parse)(struct _Itf_00040020_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_00040020_t*, bus_packet *, u16* value,u8 length);
} Itf_00040020_t;

Itf_00040020_t *itf_00040020_new(void);

#endif
