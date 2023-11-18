#ifndef __ITF_137_H__
#define __ITF_137_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u16 cnt;
    u16 time_span_ms;
    u16 channel;
} Itf_137_content_t;

typedef struct _Itf_137_t {
    Itf_137_content_t content;
    void (*packet_parse)(struct _Itf_137_t*, bus_packet *);
    void (*rtn_and_free)(struct _Itf_137_t *,bus_packet*, u32, u16, u16);
} Itf_137_t;

Itf_137_t* Itf_137_new(void);

#endif
