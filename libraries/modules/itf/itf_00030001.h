#ifndef __ITF_00030001_H__
#define __ITF_00030001_H__
#include "macro.h"
#include "bus.h"
typedef struct {
	u8 gid;
    u8 idx;
    u16 duty_cycle;
}Itf_00030001_content_t;

typedef struct _Itf_00030001_t
{
    Itf_00030001_content_t content;
    void (*packet_parse)(struct _Itf_00030001_t*, bus_packet* );
    void (*rtn_and_free)(struct _Itf_00030001_t*, bus_packet*, u8 error_code);
}Itf_00030001_t;

Itf_00030001_t *itf_00030001_new(void);
#endif

