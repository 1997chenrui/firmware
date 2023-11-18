#ifndef __ITF_00020000_H__
#define __ITF_00020000_H__
#include "macro.h"
#include "bus.h"

typedef struct {
 
    u32 m1_state;
}Itf_00020000_content_t;

typedef struct _Itf_00020000_t
{
    Itf_00020000_content_t content;
    void (*packet_parse)(struct _Itf_00020000_t*, bus_packet* );
    void (*rtn_and_free)(struct _Itf_00020000_t*, bus_packet*, u8 error_code);
}Itf_00020000_t;

Itf_00020000_t *itf_00020000_new(void);

#endif


