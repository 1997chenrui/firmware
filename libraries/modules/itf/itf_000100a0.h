#ifndef __ITF_000100a0_H__
#define __ITF_000100a0_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u8 id;
} Itf_000100a0_content_t;

typedef struct _Itf_000100a0_t{
    Itf_000100a0_content_t content;
    void (*packet_parse)(struct _Itf_000100a0_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_000100a0_t* , bus_packet *, u8 error_code);
} Itf_000100a0_t;

Itf_000100a0_t* Itf_000100a0_new(void);

#endif


