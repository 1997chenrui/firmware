#ifndef __ITF_00070011_H__
#define __ITF_00070011_H__
#include "macro.h"
#include "bus.h"
typedef struct {
    u8 idx;
    u8 persent;
    u16 time_ms;
}Itf_00070011_content_t;

typedef struct _Itf_00070011_t
{
	Itf_00070011_content_t content;
    void (*packet_parse)(struct _Itf_00070011_t *, bus_packet* );
    void (*rtn_and_free)(struct _Itf_00070011_t *, bus_packet*, u8);
}Itf_00070011_t;

Itf_00070011_t *itf_00070011_new(void);
void itf_00070011_free(Itf_00070011_t *self);
#endif

