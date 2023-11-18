#ifndef __ITF_00080000_H__
#define __ITF_00080000_H__
#include "macro.h"
#include "bus.h"

typedef struct{
    u8 id;
    u8 value;
}Itf_00080000_content_t;

Itf_00080000_content_t Itf_00080000_parse(bus_packet* packet);
void Itf_00080000_rtn(bus_packet* packet, u8 error_code);
#endif


