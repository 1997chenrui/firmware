#ifndef __ITF_00010062_H__
#define __ITF_00010062_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u8 idx;
    u8 state;
} Itf_00010062_content_t;

typedef struct _Itf_00010062_t{
    Itf_00010062_content_t content;
    void (*packet_parse)(struct _Itf_00010062_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010062_t* , bus_packet *, u8 error_code);
} Itf_00010062_t;

Itf_00010062_t* Itf_00010062_new(void);

#endif

