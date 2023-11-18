#include <stdlib.h>
#include "pc_reboot.h"
#include "itf_01000c60.h"
#include "stm32f4xx_hal.h"
#include "delayus.h"
#include "project_cfg.h"

void reboot_pc_cmd(u32 func_code, bus_packet* packet){
    switch(func_code){
	
        case 0x01000c60:{
			
            Itf_01000c60_t* cmd = itf_01000c60_new();
			
			cmd->rtn_and_free(cmd, packet, 0);
			
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);	//5v POWER DISEN
			
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);	//m-power-ctrl close
			
			OSTimeDly(500);
         
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);	//5v POWER DISEN
			
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);	//m-power-ctrl close

			NVIC_SystemReset();
			
        }
		
		break;
		
		default:ASSERT(1, "pc_reboot fun_code not find");
    }
}
