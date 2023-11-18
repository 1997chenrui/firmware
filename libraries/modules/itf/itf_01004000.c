#include "itf_01004000.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

static Itf_01004000_content_t* parse_send(bus_packet* packet);
static void del_send(Itf_01004000_content_t* send);
static void rtn(Itf_01004000_content_t* content, u32 cmd, u32 error_code);

void Itf_01004000_init(Itf_01004000_t* self){
    self->del_send = del_send;
    self->parse_send = parse_send;
    self->rtn = rtn;
}

/** Itf_01004000_t 成员函数 **/
static Itf_01004000_content_t* parse_send(bus_packet* packet){
    Itf_01004000_content_t* send = (Itf_01004000_content_t* )malloc(sizeof(Itf_01004000_content_t));
    u8 offset = 4;
    send->pc_power = ReadU16(&packet->data[offset]);
    offset += 2;
    send->motor_power = ReadU16(&packet->data[offset]);
    offset += 2;
    send->temperature_power = ReadU16(&packet->data[offset]);
    offset += 2;
    send->data_power = ReadU16(&packet->data[offset]);
    send->packet = *packet;
    return send;
}

static void del_send(Itf_01004000_content_t* send){
    if(send != NULL){
        free(send);
    }
}

static void rtn(Itf_01004000_content_t* content, u32 cmd, u32 error_code){
    u8 data[8];
    WriteU32(data, cmd);
    WriteU32(&data[4], error_code);
    BUS_ITA_RTN(content->packet, data, 8);
    del_send(content);
}


