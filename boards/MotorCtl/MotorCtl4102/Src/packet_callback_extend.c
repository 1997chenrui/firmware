#include "bus.h"
#include "hander.h"

u8 bus_ita_func_callback_extern(u32 func_code, bus_packet * packet){
    switch(func_code){
        case 0x00010032:{
            hander_cmd_run(func_code, packet);
            break;
        }
        default:return 1;
    }
    return 0;
}
