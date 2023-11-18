#ifndef __ITF_00040050_H__
#define __ITF_00040050_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 m1_id_cmd;
} Itf_00040050_content_t;

typedef struct _Itf_00040050_t {
    Itf_00040050_content_t content;
    void (*packet_parse)(struct _Itf_00040050_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_00040050_t*, bus_packet *, u16 value);
} Itf_00040050_t;

Itf_00040050_t *itf_00040050_new(void);
	
#endif
