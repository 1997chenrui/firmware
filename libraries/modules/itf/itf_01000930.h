#ifndef __ITF_01000920_H__
#define __ITF_01000920_H__
#include "bus.h"
#include "macro.h"

typedef struct {
    u8 m1_sen_grade;
} _Itf_01000930_content_t;

typedef struct _Itf_01000930_t {
    _Itf_01000930_content_t content;
    void (*packet_parse)(struct _Itf_01000930_t *, bus_packet *);
    void (*rtn_and_free)(struct _Itf_01000930_t*, bus_packet *, u8 m1_errCode);
} Itf_01000930_t;

Itf_01000930_t *itf_01000930_new(void);

#endif
