#ifndef __ITF_00040030_H__
#define __ITF_00040030_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u16 group;
} Itf_00040030_content_t;

typedef struct _Itf_00040030_t {
    Itf_00040030_content_t content;
    void (*packet_parse)(struct _Itf_00040030_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_00040030_t*, bus_packet *, u16* value,u8 length);
} Itf_00040030_t;

Itf_00040030_t *itf_00040030_new(void);

#endif
