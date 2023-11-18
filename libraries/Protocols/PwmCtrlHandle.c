#include "PwmCtrlHandle.h"
#include "cmd.h"
#include "Protocols.h"
#include "pwm_set.h"

			
void PwmCtrlCmdExcute(u32 func_code  ,  bus_packet * packet )
{
	switch(func_code)
	{
		case CMD_PWMSet:
		{
//			printf("CMD_PWMSet\r\n");
			PWM_Set_Send* set = New_PWM_Set_Send();
			PWM_Set_Return* rt = New_PWM_Set_Return();
			
			set->GetFromBytes(set,packet->data,4);
			printf("%x\r\n",set->m1_time_channle);
			printf("%d\r\n",set->m2_period);
			printf("%d\r\n",set->m3_dutyCycle);
			rt->m1_errCode = updatePwm(set->m1_time_channle,set->m2_period,set->m3_dutyCycle);//m1_time_channle: bit 0-3 定时器  bit 4-7 通道
																							  //m2_period: PWM周期 单位us
																							  //m3_dutyCycle:占空比 单位 us  （高脉冲的时间）			
//			if( set->m2_period != 0)//m2_period=0，说明是由其他TIM通过在中断处理函数中写IO来实现的PWM控制
			startPumpPwm(set->m1_time_channle);	

			
			BytesBufferNode * node =  rt->ToBytes(rt);										  
//			printf("PWMSet_errCode: %d\r\n",rt->m1_errCode);
			u8 *data = NULL;
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			free(data);
			Delete_PWM_Set_Send(set);
			Delete_PWM_Set_Return(rt);
			break;
		}
	}
	
}



