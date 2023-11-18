#include "itf_00080010.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

Itf_00080010_content_t Itf_00080010_parse(bus_packet* packet){
    u8 offset = 4;
    Itf_00080010_content_t content;
	content.group_id = packet->data[offset++];
    content.id = packet->data[offset++];
    content.color = ReadU32(&packet->data[offset]);
    return content;
}

void Itf_00080010_rtn(bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
}

