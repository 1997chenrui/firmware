#ifndef __ITF_01000b04_H__
#define __ITF_01000b04_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u8 idx;
} Itf_01000b04_content_t;

typedef struct _Itf_01000b04_t{
    Itf_01000b04_content_t content;
    void (*packet_parse)(struct _Itf_01000b04_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000b04_t* , bus_packet *, u8 error_code);
} Itf_01000b04_t;

Itf_01000b04_t* Itf_01000b04_new(void);

#endif

