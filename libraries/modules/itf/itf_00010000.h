#ifndef __ITF_00010000_H__
#define __ITF_00010000_H__
#include "macro.h"
#include "bus.h"

typedef enum{
    ITF_00010000_NO_MODIFY = 0,
    ITF_00010000_MODIFY
}Itf_00010000_cmd_e;

typedef union{
    struct
    {
		
        u8 d:7;
        u8 c:1;
    }data_u;
    u8 data;
}Itf_00010000_u8_u;

typedef union{
    struct
    {
		
        u16 d:15;
		u16 c:1;

    }data_u;
    u16 data;
}Itf_00010000_u16_u;

typedef struct {
    u8 idx;
    Itf_00010000_u8_u current_level;
    Itf_00010000_u8_u hold_level;
    Itf_00010000_u16_u subdivision;
} Itf_00010000_content_t;

typedef struct _Itf_00010000_t{
    Itf_00010000_content_t content;
    void (*packet_parse)(struct _Itf_00010000_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010000_t* , bus_packet *, u8 error_code);
} Itf_00010000_t;

Itf_00010000_t* Itf_00010000_new(void);

#endif

