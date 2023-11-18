#ifndef __ITF_00080010_H__
#define __ITF_00080010_H__
#include "macro.h"
#include "bus.h"

typedef struct{
	u8 group_id;
    u8 id;
    u32 color;
}Itf_00080010_content_t;

Itf_00080010_content_t Itf_00080010_parse(bus_packet* packet);
void Itf_00080010_rtn(bus_packet* packet, u8 error_code);
#endif


