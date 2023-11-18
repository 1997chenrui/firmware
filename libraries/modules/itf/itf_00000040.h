#ifndef __ITF_00000040_H__
#define __ITF_00000040_H__
#include "macro.h"
#include "bus.h"
typedef struct {
    u8 state;
}Itf_00000040_content_t;

typedef struct _Itf_00000040_t
{
    Itf_00000040_content_t content;
    void (*packet_parse)(struct _Itf_00000040_t*, bus_packet* );
    void (*rtn_and_free)(struct _Itf_00000040_t*, bus_packet*, u8 error_code);
}Itf_00000040_t;

Itf_00000040_t *itf_00000040_new(void);
#endif

