#ifndef __ITF_00080011_H__
#define __ITF_00080011_H__
#include "macro.h"
#include "bus.h"

typedef struct{
	u8 group_id;
    u16 id;
    u32 color;
}Itf_00080011_content_t;

Itf_00080011_content_t Itf_00080011_parse(bus_packet* packet);
void Itf_00080011_rtn(bus_packet* packet, u8 error_code);
#endif


