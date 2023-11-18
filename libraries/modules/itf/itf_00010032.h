#ifndef __ITF_00010032_H__
#define __ITF_00010032_H__
#include "macro.h"
#include "bus.h"

typedef enum {
    ITF_00010032_ABS = 0,
    ITF_00010032_REL
}Itf_00010032_cmd_e;

typedef struct {
    union
    {
        struct{
            u8 idx:4;
            u8 status:4;
        }data_u;
        u8 data;
    }idx_status;
} Itf_00010032_content_t;

typedef struct _Itf_00010032_t{
    Itf_00010032_content_t content;
    void (*packet_parse)(struct _Itf_00010032_t* , bus_packet *);
    void (*rtn_and_free)(struct _Itf_00010032_t* , bus_packet *, u8 error_code);
} Itf_00010032_t;

Itf_00010032_t* Itf_00010032_new(void);

#endif

