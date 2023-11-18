#ifndef __ITF_01000c40_H__
#define __ITF_01000c40_H__
#include "macro.h"
#include "bus.h"
typedef struct {
    union
    {
        struct{
            u8 idx:7;
            u8 enable:1;
        }split;
        u8 value;
    }idx_state;
}Itf_01000c40_content_t;

typedef struct _Itf_01000c40_t
{
    Itf_01000c40_content_t (*packet_parse)(bus_packet* );
    void (*rtn)(bus_packet*, u8 error_code);
}Itf_01000c40_t;

void itf_01000c40_init(Itf_01000c40_t* self);
#endif

