#ifndef __ITF_00030020_H__
#define __ITF_00030020_H__
#include "macro.h"
#include "bus.h"
typedef struct {
    u32 group_idx;
    u16 duty_cycle;
}Itf_00030020_content_t;

typedef struct _Itf_00030020_t
{
    Itf_00030020_content_t content;
    void (*packet_parse)(struct _Itf_00030020_t*, bus_packet* );
    void (*rtn_and_free)(struct _Itf_00030020_t*, bus_packet*, u8 error_code);
}Itf_00030020_t;

Itf_00030020_t *itf_00030020_new(void);
#endif

