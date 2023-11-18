#include "hander.h"
#include "cmd.h"
#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"
#include "bus_can_f4_hal.h"
#include "delayus.h"
#include "data_struct.h"
#include "itf_00010032.h"

#define             HANDER_TASK_START_PRIO                4u
static  OS_STK      HanderTaskStartStk[HANDER_CFG_TASK_START_STK_SIZE];
SeqQueue_Struct   HanderQueue;        //队列成员应该是命令码和send对象
Hander_thread Hander_thread_OS;

typedef enum{
    HANDER_NO_ERROR = 0,	//无错误
    HANDER_TIME_OUT = 1,	//时间超时
    HANDER_CTRL_ERROR=2,	//驱动器错误
    HANDLE_SEQUENCE_ERROR=3,//执行顺序错误
	Cannot_Complete=4, 		//遇到无法排除的错误 (需重新上电 上电后依旧无法解决的 判断为硬件异常)
	NoErrorToComplete=5,		//没有实际的错误去排除
	SVRE_ON_OUTTIME=6,		//等待抓手伺服机构ON 超时
	NoCatcah=7,				//抓动作 没有抓住 任何东西
	CatcahErr=8   			//抓取的时候 物件有倾斜的可能 （需要执行错误复位）
    
}Hander_state_e;

static bool queue_hander_pend()            
{                                        
	bus_os_pend_sem(&Hander_thread_OS.sem,0);    
	return true;
}            

static bool queue_hander_post()         
{                                        
	bus_os_post_sem(&Hander_thread_OS.sem);    
	return true;                        
}

static void HanderThread_EnQueue(u32 itf_code, bus_packet *packet_head, void *data)
{    
    Hander_Queue_Ele_t element;
    element.cmd_code = itf_code;
    element.packet_head = *packet_head;
    element.p_cmd = data;
    SeqQueue_Enter(&HanderQueue,&element);
}


void hander_cmd_run(u32 func_code  ,  bus_packet * packet )
{
	printf("hander func_code = %d \r\n",func_code);
	switch(func_code)
	{
		case 0x00010032:
		{
            Itf_00010032_t* cmd = Itf_00010032_new();
            cmd->packet_parse(cmd, packet);
			HanderThread_EnQueue(func_code, packet, cmd);
			break;
		}
	}
}


#define SVON_ON() {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);}
#define SVON_OFF() {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);}

#define SETUP_ON() {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);}
#define SETUP_OFF() {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);}
		
#define RESET_ON() {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);}
#define RESET_OFF() {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);}
		
#define DRIVE_ON() {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);}
#define DRIVE_OFF() {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);}

#define IN0_ON() {HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);}
#define IN0_OFF() {HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);}

#define IN1_ON() {HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);}
#define IN1_OFF() {HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);}

#define IN2_ON() {}
#define IN2_OFF() {}
#define IN3_ON() {}
#define IN3_OFF() {}
#define IN4_ON() {}
#define IN4_OFF() {}
#define IN5_ON() {}
#define IN5_OFF() {}



//伺服机构准备ON 为低电平（电路实际测试结果）
#define SVRE_IS_ON() HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_RESET
#define SVRE_IS_OFF() HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_SET
	
//忙信号 时为低电平  不忙 为高电平（电路实际测试结果）
#define BUSY_IS_ON() HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET
#define BUSY_IS_OFF() HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_SET

	
//触发信号为 低电平 未触发未高电平（电路实际测试结果）
#define INP_IS_ON() HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == GPIO_PIN_RESET
#define INP_IS_OFF() HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == GPIO_PIN_SET

	
	
#define ERROR_IS_ON() HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_SET
//有错误的时候 高电平 否则低电平 （电路实际测试结果）
#define ERROR_IS_OFF() HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_RESET
	
			


unsigned char record_state=0;


// 抓手动作接口
unsigned int hander_opt(unsigned char state)
{
	unsigned int timeout = 0;
	unsigned int rt=0;
	
	switch(state)
	{
		case 1:
			printf("hander reset...\r\n");
			SVON_ON();
			SETUP_OFF();
			timeout=0;
			while(!(SVRE_IS_ON())||ERROR_IS_ON()) //等待 抓手伺服机构为ON时 再开始动作 以前依靠简单延时不可靠 现修改
			{
				if(ERROR_IS_ON())
				{
									printf("err...\r\n");
					rt=HANDER_CTRL_ERROR;//返回 硬件错误 
					return rt;
				}
				if(timeout++ > 500)
				{
					printf("SVRE_ON_OUTTIME...\r\n");
					rt=SVRE_ON_OUTTIME;//返回超时 动作未执行到位
					return rt;
				}
				OSTimeDly(10);
			}
			
			OSTimeDly(60);
			SETUP_ON();
			OSTimeDly(200);
			timeout = 0;

			printf("Warit hander action...\r\n");
			
			while(!(BUSY_IS_ON())||ERROR_IS_ON()) // BUSY 等待抓手开始动作  如果没有执行动作 则等待其超时 如果报警进入报警处理
			{
				if(ERROR_IS_ON())
				{
									printf("err...\r\n");
					rt=HANDER_CTRL_ERROR;//返回 硬件错误 
					return rt;
				}
				if(timeout++ > 500)
				{
									printf("timeout...\r\n");
					rt=HANDER_TIME_OUT;//返回超时 动作未执行到位
					return rt;
				}
				OSTimeDly(10);
			}
			
			//OSTimeDly(60); // 延时必须要
			SETUP_OFF();
			timeout = 0;
				printf("Warit hander action is Ok...\r\n");
		
			while(!(BUSY_IS_OFF())||ERROR_IS_ON()) // BUSY  （动作完成）
			{				
				if(ERROR_IS_ON())
				{
									printf("err...\r\n");
					rt=HANDER_CTRL_ERROR;
					return rt;
				}
				if(timeout++ > 500)
				{				
									printf("timeout...\r\n");
					rt=HANDER_TIME_OUT;//返回超时 动作未执行到位
					return rt;
				}
				OSTimeDly(10);
			}
			record_state = 1;
			printf("hander reset successful...\r\n");
			rt= 0;
		break;
			
		case 2://报警复位		
			printf("Error rest start...\r\n");
			record_state = 2;
		
			if(ERROR_IS_ON())//首先 是错误状态 才可能 进行错误复位
			{
				printf("hander warnning reset...\r\n");
				SVON_ON();
				RESET_ON();			
				OSTimeDly(40);
				RESET_OFF();	

				rt=0;
				timeout = 0;
				OSTimeDly(500);
				
				while(ERROR_IS_ON()) //如果一直是高电平 那么是 错误复位失败
				{					
					if(timeout++ > 10)
					{
						rt= Cannot_Complete;  //遇到无法排除的错误
						printf("Error IS Not Complete...\r\n");
						return rt;
					}
					
					RESET_ON();			
					OSTimeDly(40);
					RESET_OFF();
					
					OSTimeDly(500);
					
				}		
				 printf("Error IS Completed...\r\n");

			}
			else//如果不是错误状态 
			{
				printf("No Error To Complete...\r\n");
				rt= NoErrorToComplete;  //没有实际的错误去 解决
				//rt=0;
			}
		break;

		case 3://抓手松开
		{
			if(record_state == 3)
			{
				rt=0;
				printf("already loose \r\n");
				return rt;
			}
			if(BUSY_IS_OFF()) // BUSY 先判断抓手忙不忙 如果抓手不忙
			{
				printf("hander opt3...\r\n");
				SVON_ON();	
				timeout = 0;
				DRIVE_OFF();
				while(!(SVRE_IS_ON())) //等待 抓手伺服机构为ON时 再开始动作 以前依靠简单延时不可靠 现修改
				{
					if(timeout++ > 200)
					{
						printf("SVRE_ON_OUTTIME...\r\n");
						rt=SVRE_ON_OUTTIME;//返回超时 动作未执行到位
						return rt;
					}
					OSTimeDly(10);
				}
				
				IN0_OFF();
				IN1_OFF();
				
				OSTimeDly(40); // min time = 15ms
				DRIVE_ON();

				timeout = 0;
				printf("Wait is Busy...\r\n");
				while(!(BUSY_IS_ON())||ERROR_IS_ON()) // BUSY
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}
					if(timeout++ > 500)
					{
						printf("timeout\r\n");
						rt= HANDER_TIME_OUT;  //返回超时
						return rt;
					}
					
					OSTimeDly(10);
				}
				OSTimeDly(50);
				DRIVE_OFF();				
				timeout = 0;
				
				printf("Wait is INPUT...\r\n");
				while(!(INP_IS_ON())||ERROR_IS_ON()) // INP
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}
					if(timeout++ > 500)
					{
						printf("timeout\r\n");
						return HANDER_TIME_OUT;  //返回超时
					}
					
					OSTimeDly(10);
				}
				OSTimeDly(60);
				timeout = 0;
				printf("Wait work is ok...\r\n");
				while(!(BUSY_IS_OFF())||ERROR_IS_ON()) // BUSY
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}
					if(timeout++ > 400)
					{
						printf("timeout\r\n");
						rt= HANDER_TIME_OUT;  //返回超时
						return rt;
					}	
					OSTimeDly(10);
				}
				record_state = 3;
				printf("hander opt3 successful\r\n");
				rt=0;
			}
			else
			{
				printf("hander opt3, busy status error\r\n");
				
				rt= HANDER_CTRL_ERROR;
			}
		break;	
		}
		
		case 4://抓手抓住
		{
			printf(" record_state=%d\r\n",record_state);
			
			if(record_state != 3)
			{
				rt=HANDLE_SEQUENCE_ERROR;
				printf("HANDLE_SEQUENCE_ERROR \r\n");
				return rt;
			}
			if(record_state == 4)
			{
				rt=0;
				printf("already catcah \r\n");
				return rt;
			}
			if(BUSY_IS_OFF()) // BUSY  判断抓手目前忙不忙
			{
				printf("hander opt4...\r\n");				
				SVON_ON();
				
				timeout = 0;
				DRIVE_OFF();
				
				while(!(SVRE_IS_ON())) //等待 抓手伺服机构为ON时 再开始动作 以前依靠简单延时不可靠 现修改
				{
					if(timeout++ > 200)
					{
						printf("SVRE_ON_OUTTIME...\r\n");
						rt=SVRE_ON_OUTTIME;//返回超时 动作未执行到位
						return rt;
					}
					OSTimeDly(10);
				}	
				
				
				IN0_ON();
				IN1_OFF();
				
				OSTimeDly(40); // min time = 15ms
				printf("state=4, wait busy\r\n");
				timeout = 0;
				DRIVE_ON();
				/*
				在推压运行模式下：DRIVE信号输入ON后 
								  BUSY引脚不会立即输出ON
								  要等 到达 指定推压位置下 进行推压运行后 才输出ON
				*/
				OSTimeDly(600);
				while(!(BUSY_IS_ON())||ERROR_IS_ON()) // BUSY 等抓手动作起来
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}
					if(timeout++ > 200)
					{
						printf("busy timeout\r\n");
						record_state = 4;
						rt=HANDER_TIME_OUT;
						return rt;
					}
					OSTimeDly(10);
				}
				
				OSTimeDly(60);	
				DRIVE_ON();
				
				printf("state=4, wait INP\r\n");
				timeout = 0;

				while(!(INP_IS_ON())||ERROR_IS_ON()) // INP
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						rt=CatcahErr;
						return rt;
					}
					if(timeout++ > 500)
					{
						printf("wait INP timeout\r\n");
						//如果执行到这里 可以断定为 抓手抓取内容为空
						record_state = 4;
						//rt= HANDER_TIME_OUT;  //返回超时
						rt=NoCatcah;
						return rt;
					}
					OSTimeDly(10);
				}
				OSTimeDly(60);
				timeout = 0;
				printf("Wait work is ok...\r\n");
				while(!(BUSY_IS_OFF())||ERROR_IS_ON()) // BUSY
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}					
					if(timeout++ > 400)
					{
						printf("state=4, wait BUSY timeout\r\n");
						record_state = 4;
						rt=HANDER_TIME_OUT;
						return rt;
					}
					OSTimeDly(10);
				}
				record_state = 4;
				printf("hander opt4 successful\r\n");
				rt=0;
			}
			else
			{
				printf("hander opt4 busy status error...\r\n");
				rt=0;
			}
		break;
		}
		
		case 5:
		{
			printf(" record_state=%d\r\n",record_state);
			if(record_state != 4)
			{
				rt=HANDLE_SEQUENCE_ERROR;
						return rt;
			}
			if(record_state == 5)
			{
				rt=0;
						return rt;
			}
			if(BUSY_IS_OFF()) // BUSY
			{
				printf("hander opt5...\r\n");
				SVON_ON();
				
				timeout = 0;
				DRIVE_OFF();
				while(!(SVRE_IS_ON())) //等待 抓手伺服机构为ON时 再开始动作 以前依靠简单延时不可靠 现修改
				{
					if(timeout++ > 200)
					{
						printf("SVRE_ON_OUTTIME...\r\n");
						rt=SVRE_ON_OUTTIME;//返回超时 动作未执行到位
						return rt;
					}
					OSTimeDly(10);
				}	
				

				
				IN0_OFF();
				IN1_ON();
				
				OSTimeDly(40); // min time = 15ms

				
				DRIVE_ON();
				printf("state=5, wait busy\r\n");
				timeout = 0;
				while(!(BUSY_IS_ON())||ERROR_IS_ON()) // BUSY
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}		
					if(timeout++ > 200)
					{
						printf("state=5, wait busy timeout\r\n");
						record_state = 5;
						rt=HANDER_TIME_OUT;
						return rt;
					}
					OSTimeDly(10);
				}
				
				OSTimeDly(60);
				printf("state=5, wait INP\r\n");
				timeout = 0;
				while(!(INP_IS_ON())||ERROR_IS_ON()) // INP
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}	
					if(timeout++ > 500)
					{
						printf("state=5, wait INP timeout\r\n");
						record_state = 5;
						rt=HANDER_TIME_OUT;
						rt=NoCatcah;
						return rt;
					}
					
					OSTimeDly(10);
				}
				
				OSTimeDly(60);
				timeout = 0;
								printf("Wait work is ok...\r\n");
				while(!(BUSY_IS_OFF())||ERROR_IS_ON()) // BUSY
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
							return rt;
					}	
					if(timeout++ > 500)
					{
						printf("state=5, wait BUSY timeout\r\n");
						record_state = 5;
						rt=HANDER_TIME_OUT;
						return rt;
					}
					
					OSTimeDly(10);
				}
				record_state = 5;
				printf("hander opt5 successful\r\n");
				rt=0;
			}
			else
			{
				printf("hander opt5 busy status error...\r\n");
				rt=HANDER_CTRL_ERROR;
				
			}
		break;
		}
		
		case 6://抓手松开一点点 相对
		{

			if(BUSY_IS_OFF()) // BUSY 先判断抓手忙不忙 如果抓手不忙
			{
				printf("hander opt6...\r\n");
				SVON_ON();	
				timeout = 0;
				DRIVE_OFF();
				while(!(SVRE_IS_ON())) //等待 抓手伺服机构为ON时 再开始动作 以前依靠简单延时不可靠 现修改
				{
					if(timeout++ > 200)
					{
						printf("SVRE_ON_OUTTIME...\r\n");
						rt=SVRE_ON_OUTTIME;//返回超时 动作未执行到位
						return rt;
					}
					OSTimeDly(10);
				}
				
				IN0_ON();
				IN1_ON();
				
				OSTimeDly(40); // min time = 15ms
				DRIVE_ON();

				timeout = 0;
				printf("Wait is Busy...\r\n");
				while(!(BUSY_IS_ON())||ERROR_IS_ON()) // BUSY
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}
					if(timeout++ > 500)
					{
						printf("timeout\r\n");
						rt= HANDER_TIME_OUT;  //返回超时
						return rt;
					}
					
					OSTimeDly(10);
				}
				OSTimeDly(50);
				DRIVE_OFF();				
				timeout = 0;
				
				printf("Wait is INPUT...\r\n");
				while(!(INP_IS_ON())||ERROR_IS_ON()) // INP
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}
					if(timeout++ > 500)
					{
						printf("timeout\r\n");
						return HANDER_TIME_OUT;  //返回超时
					}
					
					OSTimeDly(10);
				}
				OSTimeDly(60);
				timeout = 0;
				printf("Wait work is ok...\r\n");
				while(!(BUSY_IS_OFF())||ERROR_IS_ON()) // BUSY
				{
					if(ERROR_IS_ON())
					{
						printf("err\r\n");
						rt=HANDER_CTRL_ERROR;
						return rt;
					}
					if(timeout++ > 400)
					{
						printf("timeout\r\n");
						rt= HANDER_TIME_OUT;  //返回超时
						return rt;
					}	
					OSTimeDly(10);
				}
				record_state = 3;
				printf("hander opt6 successful\r\n");
				rt=0;
			}
			else
			{
				printf("hander opt6 , busy status error\r\n");
				
				rt= HANDER_CTRL_ERROR;
			}
		break;	
		}
		
		
		default:
		{
			rt=HANDER_CTRL_ERROR;
		break;
		}
		
	}	
	return rt;
}


//// 抓手动作接口
//unsigned int hander_opt(unsigned char state)
//{
//	unsigned int timeout = 0;
//	
//	if(state == 1)   //原点复位
//	{
//		printf("hander reset...\r\n");
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);  //SVON
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);  //SETUP
//		OSTimeDly(60);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);  //SETUP	
//		timeout = 0;
//		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET) // BUSY 等待抓手开始动作 
//		{
//			if(timeout++ > 2000)
//			{
//				printf("C11...\r\n");
//				return HANDER_TIME_OUT;  //返回超时 动作未执行到位
//			}
//			
//			OSTimeDly(10);
//		}
//		
//		OSTimeDly(60); // 延时必须要
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);  //SETUP
//		
//		timeout = 0;
//		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET) // BUSY  等待电平变高（动作完成）
//		{
//			if(timeout++ > 800)
//			{
//				printf("222...\r\n");
//				return HANDER_TIME_OUT;  //返回超时 动作未执行到位
//			}
//			
//			OSTimeDly(10);
//		}
//		record_state = 1;
//		printf("hander reset successful...\r\n");
//		return 0;
//	}
//	else if(state == 2)  // 报警复位
//	{
//		printf("hander warnning reset...\r\n");
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);  //SVON
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);  //RESET
//		
//		OSTimeDly(40);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);  //RESET
//		record_state = 2;
//		return 0;
//	}
//	else if(state == 3)  //抓手松开
//	{
//		if(record_state == 3)
//		{
//			return 0;
//		}
//		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_SET) // BUSY
//		{
//			printf("hander opt3...\r\n");
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);  //SVON
//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET); // IN5
//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
//			OSTimeDly(40); // min time = 15ms
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); //DRIVE
//			
//			timeout = 0;
//			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET) // BUSY
//			{
//				if(timeout++ > 1000)
//				{
//					return HANDER_TIME_OUT;  //返回超时
//				}
//				
//				OSTimeDly(10);
//			}
//			
//			OSTimeDly(50);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //DRIVE
//			
//			timeout = 0;
//			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == GPIO_PIN_SET) // INP
//			{
//				if(timeout++ > 1000)
//				{
//					return HANDER_TIME_OUT;  //返回超时
//				}
//				
//				OSTimeDly(10);
//			}
//			
//			OSTimeDly(60);
//			timeout = 0;
//			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET) // BUSY
//			{
//				if(timeout++ > 1000)
//				{
//					return HANDER_TIME_OUT;  //返回超时
//				}
//				
//				OSTimeDly(10);
//			}
//			record_state = 3;
//			printf("hander opt3 successful\r\n");
//			return 0;
//		}
//		else
//		{
//			printf("hander opt3, busy status error\r\n");
//			
//			return HANDER_CTRL_ERROR;
//		}
//	}
//	else if(state == 4) //抓手抓住
//	{
//		printf(" record_state=%d\r\n",record_state);
//		if(record_state != 3)
//		{
//			return HANDLE_SEQUENCE_ERROR;//抓手顺序错误（应先松再抓）
//		}
//		if(record_state == 4)
//		{
//			return 0;
//		}
//		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_SET) // BUSY
//		{
//			printf("hander opt4...\r\n");
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);  //SVON
//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET); // 执行步骤1
//			
//			OSTimeDly(40); // min time = 15ms
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); //DRIVE
//			
//			OSTimeDly(40);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //DRIVE
//			printf("state=4, wait busy\r\n");
//			timeout = 0;
//			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET) // BUSY
//			{
//				if(timeout++ > 500)
//				{
//					printf("state=4, wait busy timeout\r\n");
//					record_state = 4;
//					return HANDER_TIME_OUT;  //返回超时
//				}
//				
//				OSTimeDly(10);
//			}
//			
//			OSTimeDly(60);
////			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //DRIVE
//			
//			printf("state=4, wait INP\r\n");
//			timeout = 0;
//			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == GPIO_PIN_SET) // INP
//			{
//				if(timeout++ > 500)
//				{
//					printf("state=4, wait INP timeout\r\n");
//					record_state = 4;
//					return HANDER_TIME_OUT;  //返回超时
//				}
//				
//				OSTimeDly(10);
//			}
//			
//			OSTimeDly(60);
//			timeout = 0;
//			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET) // BUSY
//			{
//				if(timeout++ > 500)
//				{
//					printf("state=4, wait BUSY timeout\r\n");
//					record_state = 4;
//					return HANDER_TIME_OUT;  //返回超时
//				}
//				
//				OSTimeDly(10);
//			}
//			record_state = 4;
//			printf("hander opt4 successful\r\n");
//			return 0;
//		}
//		
//		else
//		{
//			printf("hander opt4 busy status error...\r\n");
//			return HANDER_CTRL_ERROR;
//		}
//		
//		

//	}
//	else if(state == 5) //抓手抓住 用于判断是否真的抓住
//		{
//		printf(" record_state=%d\r\n",record_state);
//		if(record_state != 4)
//		{
//			return HANDLE_SEQUENCE_ERROR;//抓手顺序错误（应先松再抓）
//		}
//		if(record_state == 5)
//		{
//			return 0;
//		}
//		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_SET) // BUSY
//		{
//			printf("hander opt5...\r\n");
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);  //SVON
//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET); // 执行步骤2
//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
//			OSTimeDly(40); // min time = 15ms
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); //DRIVE
//			
//			OSTimeDly(40);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //DRIVE
//			printf("state=5, wait busy\r\n");
//			timeout = 0;
//			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET) // BUSY
//			{
//				if(timeout++ > 500)
//				{
//					printf("state=5, wait busy timeout\r\n");
//					record_state = 5;
//					return HANDER_TIME_OUT;  //返回超时
//				}
//				
//				OSTimeDly(10);
//			}
//			
//			OSTimeDly(60);
////			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //DRIVE
//			
//			printf("state=5, wait INP\r\n");
//			timeout = 0;
//			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == GPIO_PIN_SET) // INP
//			{
//				if(timeout++ > 500)
//				{
//					printf("state=5, wait INP timeout\r\n");
//					record_state = 5;
//					return HANDER_TIME_OUT;  //返回超时
//				}
//				
//				OSTimeDly(10);
//			}
//			
//			OSTimeDly(60);
//			timeout = 0;
//			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET) // BUSY
//			{
//				if(timeout++ > 500)
//				{
//					printf("state=5, wait BUSY timeout\r\n");
//					record_state = 5;
//					return HANDER_TIME_OUT;  //返回超时
//				}
//				
//				OSTimeDly(10);
//			}
//			record_state = 5;
//			printf("hander opt5 successful\r\n");
//			return 0;
//		}
//		else
//		{
//			printf("hander opt5 busy status error...\r\n");
//			return HANDER_CTRL_ERROR;
//		}
//	}
//	else
//	{
//		return HANDER_CTRL_ERROR;
//	}
//}


static void hander_thread(void *arg)
{
	printf(" ###hander_thread### \r\n");
	while(1)
	{
		SeqQueue_Waite(&HanderQueue);
		Hander_Queue_Ele_t ele;						
		SeqQueue_Delete(&HanderQueue, &ele);
		
        Itf_00010032_t* pcmd = ele.p_cmd;
        
        u8 error_code=hander_opt(pcmd->content.idx_status.data_u.status);
        pcmd->rtn_and_free(pcmd, &ele.packet_head, error_code);
	}
}
		
		

void hander_init()
{
	printf(" hander_init \r\n");
	bus_create_thread(&Hander_thread_OS.tcb,hander_thread, HanderTaskStartStk, HANDER_CFG_TASK_START_STK_SIZE, HANDER_TASK_START_PRIO, NULL);
	bus_os_create_sem(&Hander_thread_OS.sem,0); 
	SeqQueue_Init(&HanderQueue,sizeof(Hander_Queue_Ele_t),&queue_hander_pend,&queue_hander_post);

}

