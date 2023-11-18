#include "itf_00020030.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

extern u8 MyAdapterID;

/** Itf_00020030_t 成员函数 **/
void Itf_00020030_send(u32 state){
    u8 data[8];
    u8 data_out;
    u32 data_out_len;
    u8 offset = 0;
    WriteU32(data, 0x00020030);
    offset+=4;
    WriteU32(&data[offset], state);
    offset+=4;
    bus_ita_hb_send(MyAdapterID, 0, sizeof(data), data, &data_out, &data_out_len);
}
