#ifndef __ITF_00080012_H__
#define __ITF_00080012_H__
#include "macro.h"
#include "bus.h"

typedef struct{
	
	u8 group_id;
    u32 id;
    u32 color;
}Itf_00080012_content_t;

Itf_00080012_content_t Itf_00080012_parse(bus_packet* packet);
void Itf_00080012_rtn(bus_packet* packet, u8 error_code);
#endif


