#include <stdlib.h>
#include "ad_signal.h"
#include "ad_signal_cfg.h"
#include "itf_00040000.h"
#include "itf_00040001.h"
#include "itf_00040010.h"
#include "itf_00040020.h"
#include "itf_00040030.h"
#include "project_cfg.h"
extern Signal_read_t ad_signal;

void ad_signal_init(){
	ad_signal_hd_init();
	printf("ad_singal_init finish\r\n");
}

Signal_read_t* signal_reader_as_ref(void){
	return &ad_signal;
}

void signal_cmd_run(u32 func_code, bus_packet* packet){
    switch(func_code){
	
        case 0x00040000:{
            Itf_00040000_t* cmd = itf_00040000_new();
            cmd->packet_parse(cmd, packet);
//			printf("0x40000_sensor_id = %d\r\n",cmd->content.sensor_id);
            ASSERT(cmd->content.sensor_id >= SIGNAL_CHANNEL_CNT, "ad signal id error");
			Signal_read_t* self = &ad_signal;
            cmd->rtn_and_free(cmd, packet, self->hd_signal_read(cmd->content.sensor_id));
        }
		break;
		
		case 0x00040001:{
            Itf_00040001_t* cmd = itf_00040001_new();
            cmd->packet_parse(cmd, packet);
//			printf("0x40001_sensor_id = %d\r\n",cmd->content.sensor_id);
            ASSERT(cmd->content.sensor_id >= SIGNAL_CHANNEL_CNT, "ad signal id error");
			Signal_read_t* self = &ad_signal;
			u32 sensor_value = self->hd_singal_read_32(cmd->content.sensor_id);
//			printf("sensor_value = %d\r\n", sensor_value);
            cmd->rtn_and_free(cmd, packet, sensor_value);
        }
		break;
		
        case 0x00040010:{
            Itf_00040010_t* cmd = itf_00040010_new();
            cmd->packet_parse(cmd, packet);
            ASSERT(cmd->content.sensor_id >= SIGNAL_CHANNEL_CNT, "ad signal id error");
			Signal_read_t* self = &ad_signal;
            cmd->rtn_and_free(cmd, packet, self->hd_signal_read_two(cmd->content.sensor_id));
        }
		break;
		
		case 0x00040020:{
            Itf_00040020_t* cmd = itf_00040020_new();
            cmd->packet_parse(cmd, packet);
            ASSERT(cmd->content.sensor_id >= SIGNAL_CHANNEL_CNT, "ad signal id error");
			Signal_read_t* self = &ad_signal;
			u16 len = 0;
            cmd->rtn_and_free(cmd, packet,self->hd_signal_read_array(cmd->content.sensor_id, &len), len);
        }
        break;
		
		case 0x00040030:{
            Itf_00040030_t* cmd = itf_00040030_new();
            cmd->packet_parse(cmd, packet);
			u8 len = 0;
			Signal_read_t* self = &ad_signal;
            cmd->rtn_and_free(cmd, packet, self->hd_group_read_by(cmd->content.group, &len), len);
        }
        break;

		default:ASSERT(1, "ad signal fun_code not find");
    }
}
