#include "itf_00040020.h"
#include "bus.h"
#include "stm32f4xx.h"
#include "AD7699_task.h"
#include "../../../libraries/drivers/AD7699.h"
#include "AD7699_cfg.h"

void Ad_cmd_run(u32 func_code, bus_packet* packet){
    switch(func_code){
     
		
		case 0x00040020:{
            Itf_00040020_t* cmd = itf_00040020_new();
            cmd->packet_parse(cmd, packet);
			u16 datatest[ad7966_cfg[0].channel_cnt];
			AD7699AllRead(&ad7966_cfg[0],datatest);
			
            cmd->rtn_and_free(cmd, packet,(ad7966_cfg[0].adData),(sizeof(ad7966_cfg[0].adData)/sizeof(u16)));//·¢ËÍÊý¾Ý
        }
        break;
		default:ASSERT(1, "ad signal fun_code not find");
    }
}
