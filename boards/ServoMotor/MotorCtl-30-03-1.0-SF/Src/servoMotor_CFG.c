#include "servoMotor_CFG.h"
#include "servoMotor.h"
#include "my_main.h"


#define DEBUG_LOG_DEBUG(s,args...) printf(s,##args)
#define DEBUG_LOG_INFO(s,args...)  printf(s,##args)
#define DEBUG_LOG_WARN(s,args...)
#define DEBUG_LOG_ERROR(s,args...)




//驱动使用到的硬件SPI
extern SPI_HandleTypeDef hspi1;  
extern void delayUs(u16 us);


//驱动器列表
TMC4361TypeDef tmc4361List[SERVOMOTOR_NUM];

//伺服电机列表
SMotor sMotor[SERVOMOTOR_NUM];

/*
*********************************************************************************************************
*                                    先配置TMC4361驱动器
*********************************************************************************************************
*/	

//实现spi
void SpiWR_0(uint8_t * data)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
	delayUs(20);
	HAL_SPI_TransmitReceive(&hspi1,data,data,5,0xFFFF);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	delayUs(20);

	
}

//电机紧急制动配置
void TMC4361A_NFREEZE_0(void)
{
	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_RESET);
	delayUs(20);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_SET);
}




//实现spi
void SpiWR_1(uint8_t * data)
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);
	delayUs(20);
	HAL_SPI_TransmitReceive(&hspi1,data,data,5,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	delayUs(20);
}
 


void TMC4361A_NFREEZE_1(void)
{

	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET);
	delayUs(20);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_SET);
}



//实现spi
void SpiWR_2(uint8_t * data)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
	delayUs(20);
	
	CPU_SR_ALLOC();
	OS_ENTER_CRITICAL();
	HAL_SPI_TransmitReceive(&hspi1,data,data,5,0xFFFF);
	OS_EXIT_CRITICAL();
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	delayUs(20);
}
 

void TMC4361A_NFREEZE_2(void)
{

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
	delayUs(20);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
}



//实现spi
void SpiWR_3(uint8_t * data)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
	delayUs(20);
	CPU_SR_ALLOC();
	OS_ENTER_CRITICAL();
	HAL_SPI_TransmitReceive(&hspi1,data,data,5,0xFFFF);
	OS_EXIT_CRITICAL();
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	delayUs(20);
}
 


void TMC4361A_NFREEZE_3(void)
{

	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
	delayUs(20);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
}






void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);//
}
void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);//
}

void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);//
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);//
}


//位置到达中断和驱动器事件触发中断
void EXTI9_5_IRQHandler(void)
{

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	
	
}
void EXTI15_10_IRQHandler(void)
{

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

Add_Servomotor_Flag(0);
Add_Servomotor_Flag(1);
Add_Servomotor_Flag(2);
Add_Servomotor_Flag(3);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
	case GPIO_PIN_9://电机0到达
	Touch_Servomotor_Target(0);
	break;
	case GPIO_PIN_10://电机0中断事件
	Touch_Servomotor_Event(0);
	break;
	case GPIO_PIN_4:	// 电机0的复位开关事件
	Touch_Servomotor_Zero(0);
	break;
	

	
	case GPIO_PIN_14://电机1到达
	TMC4361_Target_Flush(&tmc4361List[1]);
	Touch_Servomotor_Target(1);
	break;
	case GPIO_PIN_15://电机1中断事件
	Touch_Servomotor_Event(1);
	break;
	case GPIO_PIN_3://电机1复位开关事件
	Touch_Servomotor_Zero(1);
	break;
	
	case GPIO_PIN_7://电机1到达
	Touch_Servomotor_Target(2);
	break;
	case GPIO_PIN_8://电机1中断事件
	Touch_Servomotor_Event(2);
	break;
	case GPIO_PIN_2://电机1复位开关事件
	Touch_Servomotor_Zero(2);
	break;  
	

	case GPIO_PIN_11://电机1到达
	Touch_Servomotor_Target(3);
	servomotor_3_TargetFlg=true;
	break;
	case GPIO_PIN_13://电机1中断事件
	Touch_Servomotor_Event(3);
	break;
	case GPIO_PIN_1://电机1复位开关事件
	Touch_Servomotor_Zero(3);
	break;  
	
	
	case GPIO_PIN_5:
		//移动到事件3停止

		break;
		
  default:
    break;
  }
    
}



//伺服电机驱动器配置
TMC4361TypeDef tmc4361List[SERVOMOTOR_NUM]=
{
	{
		.spi.spiWR=SpiWR_0,
		.TMC4361A_NFREEZE=TMC4361A_NFREEZE_0
	},
	{
		.spi.spiWR=SpiWR_1,
		.TMC4361A_NFREEZE=TMC4361A_NFREEZE_1
	},
	{
		.spi.spiWR=SpiWR_2,
		.TMC4361A_NFREEZE=TMC4361A_NFREEZE_2
	},
	{
		.spi.spiWR=SpiWR_3,
		.TMC4361A_NFREEZE=TMC4361A_NFREEZE_3
	}
};

const TMC4361CFG tmc4361cfgList[SERVOMOTOR_NUM]=
{
	{
	 .setp_fs_per_rev=200,//电机旋转一周需要多少整步 
	 .setp_per_fs=0,//电机细分 0x00=256
	 .ramp_reverse_motor_dir=false,//是否改变电机方向
	 .enc_in_invert_dir=true,//是否反向编码器方向
	 .enc_in_res=4096,//编码器的每圈步进数  0x4040000|
	 .event_intr_conf=0,//	TMC4361A_CL_FIT_MASK|TMC4361A_CL_MAX_MASK,//开启的中断事件
	 .cl_imin=10,//闭环最小电流调节值
	 .cl_imax=200//闭环最大电流调节值	
	
	},
	{
	 .setp_fs_per_rev=200,//电机旋转一周需要多少整步 
	 .setp_per_fs=0,//电机细分 0x00=256
	 .ramp_reverse_motor_dir=false,//是否改变电机方向
	 .enc_in_invert_dir=true,//是否反向编码器方向
	 .enc_in_res=4096,//编码器的每圈步进数  0x4040000|
	 .event_intr_conf=0,//	TMC4361A_CL_FIT_MASK|TMC4361A_CL_MAX_MASK,//开启的中断事件
	 .cl_imin=10,//闭环最小电流调节值
	 .cl_imax=200//闭环最大电流调节值	
	
	},
	{
	 .setp_fs_per_rev=200,//电机旋转一周需要多少整步 
	 .setp_per_fs=0,//电机细分 0x00=256
	 .ramp_reverse_motor_dir=false,//是否改变电机方向
	 .enc_in_invert_dir=true,//是否反向编码器方向
	 .enc_in_res=4096,//编码器的每圈步进数  0x4040000|
	 .event_intr_conf=0,//	TMC4361A_CL_FIT_MASK|TMC4361A_CL_MAX_MASK,//开启的中断事件
	 .cl_imin=10,//闭环最小电流调节值
	 .cl_imax=200//闭环最大电流调节值	
	
	},
	{
	 .setp_fs_per_rev=200,//电机旋转一周需要多少整步 
	 .setp_per_fs=0,//电机细分 0x00=256
	 .ramp_reverse_motor_dir=false,//是否改变电机方向
	 .enc_in_invert_dir=true,//是否反向编码器方向
	 .enc_in_res=4096,//编码器的每圈步进数  0x4040000|
	 .event_intr_conf=0,//	TMC4361A_CL_FIT_MASK|TMC4361A_CL_MAX_MASK,//开启的中断事件
	 .cl_imin=10,//闭环最小电流调节值
	 .cl_imax=200//闭环最大电流调节值	
	
	}
};

const TMC4361A_REARDRIVE_REG rearDriveReg[SERVOMOTOR_NUM][12]=
{
	{
		{.adder=0x00,.data=0x12200},
		{.adder=0x10,.data=0x00031905},//驱动电流等
		{.adder=0x11,.data=0x0000000A},//TZEROWAIT
		{.adder=0x13,.data=20},//TPWMTHRS
		{.adder=0x14,.data=120},//TCOOLTHRS
		{.adder=0x15,.data=0},//THIGH
		{.adder=0x33,.data=0},//VDCMIN
		{.adder=0x6c,.data=0x00010223},//斩波器和驱动器配置 CHOPCONF
		{.adder=0x6d,.data=1<<24|0<<16 | 10<<8 | 0},//COOLCONF	
		{.adder=0x6E,.data=0x00070025},//DCCTRL
		{.adder=0x70,.data=0x000404FF},//PWMCONF
		{.adder=0xFF,.data=0x00}
	},
	{
		{.adder=0x00,.data=0x12200},
		{.adder=0x10,.data=0x00031905},//驱动电流等
		{.adder=0x11,.data=0x0000000A},//TZEROWAIT
		{.adder=0x13,.data=20},//TPWMTHRS
		{.adder=0x14,.data=200},//TCOOLTHRS
		{.adder=0x15,.data=50},//THIGH
		{.adder=0x33,.data=500<<8},//VDCMIN
		{.adder=0x6c,.data=0x00010223},//斩波器和驱动器配置 CHOPCONF
		{.adder=0x6d,.data=1<<24|0<<16 | 10<<8 | 0},//COOLCONF
		{.adder=0x6E,.data=0x00070025},//DCCTRL
		{.adder=0x70,.data=0x000404FF},//PWMCONF
		{.adder=0xFF,.data=0x00}
	},
	{
		{.adder=0x00,.data=0x12200},
		{.adder=0x10,.data=0x00031905},//驱动电流等
		{.adder=0x11,.data=0x0000000A},//TZEROWAIT
		{.adder=0x13,.data=20},//TPWMTHRS
		{.adder=0x14,.data=200},//TCOOLTHRS
		{.adder=0x15,.data=50},//THIGH
		{.adder=0x33,.data=500<<8},//VDCMIN
		{.adder=0x6c,.data=0x00010223},//斩波器和驱动器配置 CHOPCONF
		{.adder=0x6d,.data=1<<24|0<<16 | 10<<8 | 0},//COOLCONF
		{.adder=0x6E,.data=0x00070025},//DCCTRL
		{.adder=0x70,.data=0x000404FF},//PWMCONF
		{.adder=0xFF,.data=0x00}
	},
	{
		{.adder=0x00,.data=0x12200},
		{.adder=0x10,.data=0x00031905},//驱动电流等
		{.adder=0x11,.data=0x0000000A},//TZEROWAIT
		{.adder=0x13,.data=20},//TPWMTHRS
		{.adder=0x14,.data=200},//TCOOLTHRS
		{.adder=0x15,.data=50},//THIGH
		{.adder=0x33,.data=500<<8},//VDCMIN
		{.adder=0x6c,.data=0x00010223},//斩波器和驱动器配置 CHOPCONF
		{.adder=0x6d,.data=1<<24|0<<16 | 10<<8 | 0},//COOLCONF
		{.adder=0x6E,.data=0x00070025},//DCCTRL
		{.adder=0x70,.data=0x000404FF},//PWMCONF
		{.adder=0xFF,.data=0x00}
	}
};
 


	
//驱动器初始化
static void TMC4361A_CFG_INIT()
{
//外部中断配置
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  //位置到达中断
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  //事件中断
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  
  //复位开关中断
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);  
 

  
  //位置到达中断
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  //事件中断
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  //复位开关中断
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);  
  
  
    //位置到达中断
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  //事件中断
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  //复位开关中断
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);  
 

    //位置到达中断
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  //事件中断
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  //复位开关中断
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);  
  

    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);   
  
  for(uint8_t i=0;i<SERVOMOTOR_NUM;i++)
  { 
	 tmc4361List[i].cfg=&tmc4361cfgList[i];
	 tmc4361List[i].rear_reg=&rearDriveReg[i][0];
	 TMC4361_SoftRest(&tmc4361List[i]);
	 osDelay(200);	  
	 TMC4361A_CFG_Init(&tmc4361List[i]);
  }

}

//伺服电机配置
SMotorCfg sMotorCfgList[SERVOMOTOR_NUM]=
{
	{
		.restEventPol=0,//复位电平
		.defaultDir=!1,//默认电机方向
		.step_ratio=5.468,
		.zero_distance=-4.5,
		.max_distance_reverse=0.5,
		.max_distance=175
	},
	{
		.restEventPol=0,
		.defaultDir=!0,
		.step_ratio=5.468,
		.zero_distance=-4.5,
		.max_distance_reverse=0.5,
		.max_distance=50
	},
	{
		.restEventPol=0,
		.defaultDir=!0,
		.step_ratio=5.468,
		.zero_distance=-4.5,
		.max_distance_reverse=0.5,
		.max_distance=8
	},
	{
		.restEventPol=0,
		.defaultDir=!1,
		.step_ratio=5.468,
		.zero_distance=-4.5,
		.max_distance=39.5
	}
	
};



bool TMC4361A_ZeroSwitchStateGet_0(void)
{
	return (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4));
}

bool TMC4361A_ZeroSwitchStateGet_1(void)
{
	return (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3));
}

bool TMC4361A_ZeroSwitchStateGet_2(void)
{
	return (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2));
}
bool TMC4361A_ZeroSwitchStateGet_3(void)
{
	return (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1));
}



void EventScan0(uint32_t * exflag,uint8_t* inflag)
{

	Update_Servomotor_Target(0);	
	Update_Servomotor_Event(0);
	Update_Servomotor_Zero(0);
	
	Update_Servomotor_UserEvent(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5)==GPIO_PIN_SET,3);
	

}

void EventScan1(uint32_t * exflag,uint8_t* inflag)
{

	Update_Servomotor_Target(1);	
	Update_Servomotor_Event(1);
	Update_Servomotor_Zero(1);

}

void EventScan2(uint32_t * exflag,uint8_t* inflag)
{

	Update_Servomotor_Target(2);	
	Update_Servomotor_Event(2);
	Update_Servomotor_Zero(2);

}

void EventScan3(uint32_t * exflag,uint8_t* inflag)
{

	Update_Servomotor_Target(3);	
	Update_Servomotor_Event(3);
	Update_Servomotor_Zero(3);

}

void ServoMotorInit(void)
{
	TMC4361A_CFG_INIT();
	
	for(uint8_t i=0;i<SERVOMOTOR_NUM;i++)
	{
		sMotor[i].drive=&tmc4361List[i];
		sMotor[i].cfg=&sMotorCfgList[i];
	}
	
	
	sMotor[0].sMotorGetRestSwitchState=TMC4361A_ZeroSwitchStateGet_0;
	sMotor[1].sMotorGetRestSwitchState=TMC4361A_ZeroSwitchStateGet_1;
	sMotor[2].sMotorGetRestSwitchState=TMC4361A_ZeroSwitchStateGet_2;
	sMotor[3].sMotorGetRestSwitchState=TMC4361A_ZeroSwitchStateGet_3;
	
	sMotor[0].event.scanEvent=EventScan0;
	sMotor[1].event.scanEvent=EventScan1;

	sMotor[2].event.scanEvent=EventScan2;
	sMotor[3].event.scanEvent=EventScan3;

	

	
	SMotorTask_Static.sMotorCount=SERVOMOTOR_NUM;
	SMotorTask_Static.sMotorList=sMotor;
	SMotorTask_Static.sMotorMonitorStep=0;
	
	
	
	
	  HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);
	  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	
	  HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
	  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	
	  HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
	  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	
	  HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
	  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	  
	  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
	  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	  
	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	  

	
	ServoMotorTaskInit();
	
	
}
  


