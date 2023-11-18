#ifndef __ITF_00070001_H__
#define __ITF_00070001_H__
#include "macro.h"
#include "bus.h"
typedef struct {
    u8 idx;
    u8 persent;
}Itf_00070001_content_t;

typedef struct _Itf_00070001_t {
    Itf_00070001_content_t content;
    void (*packet_parse)(struct _Itf_00070001_t* , bus_packet* );
    void (*rtn_and_free)(struct _Itf_00070001_t* , bus_packet*, u8 error_code);
}Itf_00070001_t;

Itf_00070001_t *itf_00070001_new(void);
#endif

