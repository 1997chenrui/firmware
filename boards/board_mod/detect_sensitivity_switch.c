#include "io_mod.h"
#include "itf_01000930.h"
#include "bus.h"
#include "stm32f4xx.h"
#include "project_cfg.h"
#include "detect_sensitivity_switch.h"

void detect_sen_switch(u8 ctrl_switch)
{
	if(ctrl_switch == 0)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
	}
}

void detect_sensitivity_cmd_run(u32 itf_code, bus_packet* packet){
    switch(itf_code){
        case 0x01000930:{
            //TODO::优化
            Itf_01000930_t* cmd = itf_01000930_new();
            cmd->packet_parse(cmd, packet);
			detect_sen_switch(cmd->content.m1_sen_grade);
            cmd->rtn_and_free(cmd, packet, 0);
            break;
        }
	}
}

