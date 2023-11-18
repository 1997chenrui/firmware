#ifndef __ITF_01000c31_H__
#define __ITF_01000c31_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u16 lamp_id;
} Itf_01000c31_content_t;

typedef struct _Itf_01000c31_t{
    Itf_01000c31_content_t content;
    void (*packet_parse)(struct _Itf_01000c31_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000c31_t* , bus_packet *, u16* value_array, u16 value_len);

} Itf_01000c31_t;

Itf_01000c31_t* Itf_01000c31_new(void);

#endif


