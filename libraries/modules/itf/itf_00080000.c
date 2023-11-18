#include "itf_00080000.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

Itf_00080000_content_t Itf_00080000_parse(bus_packet* packet){
    u8 offset = 4;
    Itf_00080000_content_t content;
    content.id = packet->data[offset];
    offset++;
    content.value = packet->data[offset];
    return content;
}

void Itf_00080000_rtn(bus_packet* packet, u8 error_code){
    BUS_ITA_RTN((*packet), &error_code, 1);
}

