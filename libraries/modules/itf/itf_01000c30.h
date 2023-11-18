#ifndef __ITF_01000c30_H__
#define __ITF_01000c30_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u8 id;
    f32 speed;
    u16 rotate_ms;
    u16 time_spam_ms;
} Itf_01000c30_content_t;

typedef struct _Itf_01000c30_t{
    Itf_01000c30_content_t content;
    void (*packet_parse)(struct _Itf_01000c30_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000c30_t* , bus_packet *, u16* value_array, u16 value_len);
} Itf_01000c30_t;

Itf_01000c30_t* Itf_01000c30_new(void);

#endif

