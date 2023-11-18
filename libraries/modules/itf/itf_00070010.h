#ifndef __ITF_00070010_H__
#define __ITF_00070010_H__
#include "macro.h"
#include "bus.h"
typedef struct {
    u8 idx;
    f32 voltage;
    u16 time_ms;
}Itf_00070010_content_t;

typedef struct _Itf_00070010_t
{
    Itf_00070010_content_t (*packet_parse)(bus_packet* );
    void (*rtn)(bus_packet*, u8 error_code);
}Itf_00070010_t;

Itf_00070010_t *itf_00070010_new(void);
void itf_00070010_free(Itf_00070010_t *self);
#endif

