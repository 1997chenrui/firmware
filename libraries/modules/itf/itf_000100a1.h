#ifndef __ITF_000100a1_H__
#define __ITF_000100a1_H__
#include "macro.h"
#include "bus.h"

typedef struct{
    u8 id;
    u8 channel;
}Itf_000100a1_content_t;

typedef struct _Itf_000100a1_t{
    Itf_000100a1_content_t content;
    void (*packet_parse)(struct _Itf_000100a1_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_000100a1_t* , bus_packet *, u8 error_code);
} Itf_000100a1_t;

Itf_000100a1_t* Itf_000100a1_new(void);

#endif


