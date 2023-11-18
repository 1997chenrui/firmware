#ifndef __ITF_01000920_H__
#define __ITF_01000920_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 m1_reserve;
} _Itf_01000920_content_t;

typedef struct _Itf_01000920_t {
    _Itf_01000920_content_t content;
    void (*packet_parse)(struct _Itf_01000920_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000920_t*, bus_packet *, u8 m1_errCode);
} Itf_01000920_t;

Itf_01000920_t *itf_01000920_new(void);

#endif
