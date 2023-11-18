#ifndef __ITF_00070003_H__
#define __ITF_00070003_H__
#include "macro.h"
#include "bus.h"
typedef struct {
    u8 idx;
    u16 code;
}Itf_00070003_content_t;

typedef struct _Itf_00070003_t {
    Itf_00070003_content_t content;
    void (*packet_parse)(struct _Itf_00070003_t* , bus_packet* );
    void (*rtn_and_free)(struct _Itf_00070003_t* , bus_packet*, u8 error_code);
}Itf_00070003_t;

Itf_00070003_t *itf_00070003_new(void);
#endif

