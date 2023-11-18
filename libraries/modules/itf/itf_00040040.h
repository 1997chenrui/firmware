#ifndef __ITF_00040040_H__
#define __ITF_00040040_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 m1_id;
} Itf_00040040_content_t;

typedef struct _Itf_00040040_t {
    Itf_00040040_content_t content;
    void (*packet_parse)(struct _Itf_00040040_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_00040040_t*, bus_packet *, u16* value, u16 value_length);
} Itf_00040040_t;

Itf_00040040_t *itf_00040040_new(void);
	
#endif
