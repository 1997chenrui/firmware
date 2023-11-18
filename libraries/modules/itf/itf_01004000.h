#ifndef __ITF_01004000_H__
#define __ITF_01004000_H__
#include "macro.h"
#include "bus.h"

typedef struct{
    bus_packet packet;
    u16 pc_power;
    u16 motor_power;
    u16 temperature_power;
    u16 data_power;
}Itf_01004000_content_t;

typedef struct _Itf_01004000_t
{
    Itf_01004000_content_t* (*parse_send)(bus_packet* );
    void (*del_send)(Itf_01004000_content_t* send);
    void (*rtn)(Itf_01004000_content_t*, u32 cmd, u32 error_code);
}Itf_01004000_t;

void Itf_01004000_init(Itf_01004000_t* self);

#endif

