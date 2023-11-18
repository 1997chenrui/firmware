#ifndef __ITF_00020001_H__
#define __ITF_00020001_H__
#include "macro.h"
#include "bus.h"

typedef struct {
    u8 group_idx;
    u32 m1_state;
}Itf_00020001_content_t;

typedef struct _Itf_00020001_t
{
    Itf_00020001_content_t content;
    void (*packet_parse)(struct _Itf_00020001_t*, bus_packet* );
    void (*rtn_and_free)(struct _Itf_00020001_t*, bus_packet*, u8 error_code);
}Itf_00020001_t;

Itf_00020001_t *itf_00020001_new(void);

#endif


