#ifndef __ITF_00030010_H__
#define __ITF_00030010_H__
#include "macro.h"
#include "bus.h"
typedef struct {
    u8 idx;
    u16 duty_cycle;
}Itf_00030010_content_t;

typedef struct _Itf_00030010_t
{
    Itf_00030010_content_t content;
    void (*packet_parse)(struct _Itf_00030010_t*, bus_packet* );
    void (*rtn_and_free)(struct _Itf_00030010_t*, bus_packet*, u8 error_code);
}Itf_00030010_t;

Itf_00030010_t *itf_00030010_new(void);
#endif

