#ifndef __ITF_00060010_H__
#define __ITF_00060010_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u32 state;
} itf_00060010_content_t;

typedef struct _Itf_00060010_t {
    itf_00060010_content_t content;
    void (*packet_parse)(struct _Itf_00060010_t*, bus_packet *);
    void (*rtn_and_free)(struct _Itf_00060010_t*, bus_packet *, u8 error_code);
} Itf_00060010_t;

Itf_00060010_t *itf_00060010_new(void);

#endif
