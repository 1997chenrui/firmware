#include "itf_00020050.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

u8 Itf_00020050_parse(bus_packet* packet){
    return packet->data[4];
}

void Itf_00020050_rtn(bus_packet* packet, u32 state){
    u8 data[4];
    WriteU32(data, state);
    BUS_ITA_RTN((*packet), data, sizeof(u32));
}

