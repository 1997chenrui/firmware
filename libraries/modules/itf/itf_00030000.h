#ifndef __ITF_00030000_H__
#define __ITF_00030000_H__
#include "macro.h"
#include "bus.h"
typedef struct {
    u8 idx;
    u16 duty_cycle;
}Itf_00030000_content_t;

typedef struct _Itf_00030000_t
{
    Itf_00030000_content_t content;
    void (*packet_parse)(struct _Itf_00030000_t*, bus_packet* );
    void (*rtn_and_free)(struct _Itf_00030000_t*, bus_packet*, u8 error_code);
}Itf_00030000_t;

Itf_00030000_t *itf_00030000_new(void);
#endif

