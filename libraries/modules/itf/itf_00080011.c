#include "itf_00080011.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

Itf_00080011_content_t Itf_00080011_parse(bus_packet* packet){
    u8 offset = 4;
    Itf_00080011_content_t content;
	content.group_id = packet->data[offset++];
    content.id = ReadU16(&packet->data[offset]);
    offset+=2;
    content.color = ReadU32(&packet->data[offset]);
    return content;
}

void Itf_00080011_rtn(bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
}
