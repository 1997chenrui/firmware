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
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET);
	delayUs(20);
	CPU_SR_ALLOC();
	OS_ENTER_CRITICAL();
	HAL_SPI_TransmitReceive(&hspi1,data,data,5,0xFFFF);
	OS_EXIT_CRITICAL();
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	delayUs(20);

	
}

//电机紧急制动配置
void TMC4361A_NFREEZE_0(void)
{
	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
	delayUs(20);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
}




//实现spi
void SpiWR_1(uint8_t * data)
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);
	delayUs(20);
	CPU_SR_ALLOC();
	OS_ENTER_CRITICAL();
	HAL_SPI_TransmitReceive(&hspi1,data,data,5,0xFFFF);
	OS_EXIT_CRITICAL();
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
	delayUs(20);
	
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
	delayUs(20);
}
 


void TMC4361A_NFREEZE_1(void)
{

	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
	delayUs(20);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
}


//实现spi
void SpiWR_2(uint8_t * data)
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
	delayUs(20);
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
	delayUs(20);
	CPU_SR_ALLOC();
	OS_ENTER_CRITICAL();
	HAL_SPI_TransmitReceive(&hspi1,data,data,5,0xFFFF);
	OS_EXIT_CRITICAL();
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	delayUs(20);
	
	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
	delayUs(20);
}
 


void TMC4361A_NFREEZE_2(void)
{

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
	delayUs(20);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
}



//位置到达中断和驱动器事件触发中断
void EXTI9_5_IRQHandler(void)
{

	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);

}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);//
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

void EXTI15_10_IRQHandler(void)
{

	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
	


}

volatile static bool servomotor_0_ZeroFlg=false;
volatile static bool servomotor_1_ZeroFlg=false;
volatile static bool servomotor_2_ZeroFlg=false;



volatile static bool servomotor_0_TargetFlg=false;
volatile static bool servomotor_1_TargetFlg=false;
volatile static bool servomotor_2_TargetFlg=false;

volatile static bool servomotor_0_EventFlg=false;
volatile static bool servomotor_1_EventFlg=false;
volatile static bool servomotor_2_EventFlg=false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
	case GPIO_PIN_1://电机0到达
	TMC4361_Target_Flush(&tmc4361List[0]);
	servomotor_0_TargetFlg=true;
	break;
	case GPIO_PIN_0://电机0中断事件
	servomotor_0_EventFlg=true;
	break;
	case GPIO_PIN_5:	// 电机0的复位开关事件
	servomotor_0_ZeroFlg=true;
	break;
	

	
	case GPIO_PIN_3://电机1到达
	TMC4361_Target_Flush(&tmc4361List[1]);
	servomotor_1_TargetFlg=true;
	break;
	case GPIO_PIN_2://电机1中断事件
	servomotor_1_EventFlg=true;
	break;
	case GPIO_PIN_4://电机1复位开关事件
	//ServoMotorEventSet_external(1,0x01<<1);
	servomotor_1_ZeroFlg=true;
	break;
	
	case GPIO_PIN_7://电机1到达
	TMC4361_Target_Flush(&tmc4361List[2]);
	servomotor_2_TargetFlg=true;
	break;
	case GPIO_PIN_6://电机1中断事件
	servomotor_2_EventFlg=true;
	break;
	case GPIO_PIN_15://电机1复位开关事件
	//ServoMotorEventSet_external(2,0x01<<1);
	servomotor_2_ZeroFlg=true;
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
	}
};

const TMC4361CFG tmc4361cfgList[SERVOMOTOR_NUM]=
{
	{
	 .setp_fs_per_rev=200,//电机旋转一周需要多少整步 
	 .setp_per_fs=0,//电机细分 0x00=256
	 .ramp_reverse_motor_dir=false,//是否改变电机方向
	 .enc_in_invert_dir=true,//是否反向编码器方向
	 .enc_in_res=1600,//编码器的每圈步进数  0x4040000|
	 .event_intr_conf=	0,//TMC4361A_CL_FIT_MASK|TMC4361A_CL_MAX_MASK,//开启的中断事件
	 .cl_imin=10,//闭环最小电流调节值
	 .cl_imax=200//闭环最大电流调节值	
	},
	{
	 .setp_fs_per_rev=200,//电机旋转一周需要多少整步 
	 .setp_per_fs=0,//电机细分 0x00=256
	 .ramp_reverse_motor_dir=false,//是否改变电机方向
	 .enc_in_invert_dir=true,//是否反向编码器方向
	 .enc_in_res=1600,//编码器的每圈步进数  0x4040000|
	 .event_intr_conf=	0,//TMC4361A_CL_FIT_MASK|TMC4361A_CL_MAX_MASK,//开启的中断事件
	 .cl_imin=10,//闭环最小电流调节值
	 .cl_imax=200//闭环最大电流调节值	
	},
	{
	 .setp_fs_per_rev=200,//电机旋转一周需要多少整步 
	 .setp_per_fs=0,//电机细分 0x00=256
	 .ramp_reverse_motor_dir=false,//是否改变电机方向
	 .enc_in_invert_dir=true,//是否反向编码器方向
	 .enc_in_res=1600,//编码器的每圈步进数  0x4040000|
	 .event_intr_conf=	0,//TMC4361A_CL_FIT_MASK|TMC4361A_CL_MAX_MASK,//开启的中断事件
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
	}
};
 


	
//驱动器初始化
static void TMC4361A_CFG_INIT()
{
//外部中断配置
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  //位置到达中断
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  //事件中断
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  
  //复位开关中断
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
 

  
  //位置到达中断
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  //事件中断
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  //复位开关中断
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
  
  
    //位置到达中断
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  //事件中断
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  //复位开关中断
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);  


//分样臂 和 穿刺臂 事件同步
	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;

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
		.defaultDir=1,//默认电机方向
		.step_ratio=5.468,
		.zero_distance=-4.5,
		.max_distance=525,
		.max_distance_reverse=1
	},
	{
		.restEventPol=0,
		.defaultDir=0,
		.step_ratio=5.468,
		.zero_distance=-4.5,
		.max_distance=670.7,
		.max_distance_reverse=1
	},
	{
		.restEventPol=0,
		.defaultDir=0,
		.step_ratio=5.468,
		.zero_distance=-4.5,
		.max_distance=661.8,
		.max_distance_reverse=1
	}
	
};



bool TMC4361A_ZeroSwitchStateGet_0(void)
{
	return (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5));
}

bool TMC4361A_ZeroSwitchStateGet_1(void)
{
	return (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4));
}

bool TMC4361A_ZeroSwitchStateGet_2(void)
{
	return (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15));
}


void EventScan0(uint32_t * exflag,uint8_t* inflag)
{

	if(servomotor_0_TargetFlg==true)
	{
		servomotor_0_TargetFlg=false;
		
		SetServoMotorExEvent(0);	
	}
	if(servomotor_0_EventFlg==true)
	{
		servomotor_0_EventFlg=false;
		SetServoMotorInEvent();
	}
	
	if(servomotor_0_ZeroFlg==true)
	{
		servomotor_0_ZeroFlg=false;
		SetServoMotorExEvent(1);
	}	
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12)==GPIO_PIN_RESET)
	{
		SetServoMotorExEvent(3);
	}
	else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13)==GPIO_PIN_RESET)
	{
		SetServoMotorExEvent(4);
	}
	
}


void EventScan1(uint32_t * exflag,uint8_t* inflag)
{

	if(servomotor_1_TargetFlg==true)
	{
		servomotor_1_TargetFlg=false;
		
		SetServoMotorExEvent(0);	
	}
	if(servomotor_1_EventFlg==true)
	{
		servomotor_1_EventFlg=false;
		SetServoMotorInEvent();
	}
	
	if(servomotor_1_ZeroFlg==true)
	{
		servomotor_1_ZeroFlg=false;
		SetServoMotorExEvent(1);
	}	
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12)==GPIO_PIN_RESET)
	{
		SetServoMotorExEvent(3);
	}
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13)==GPIO_PIN_RESET)
	{
		SetServoMotorExEvent(4);
	}

}

void EventScan2(uint32_t * exflag,uint8_t* inflag)
{
	if(servomotor_2_TargetFlg==true)
	{
		servomotor_2_TargetFlg=false;
		
		SetServoMotorExEvent(0);	
	}
	if(servomotor_2_EventFlg==true)
	{
		servomotor_2_EventFlg=false;
		SetServoMotorInEvent();
	}
	
	if(servomotor_2_ZeroFlg==true)
	{
		servomotor_2_ZeroFlg=false;
		SetServoMotorExEvent(1);
	}	
	 if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12)==GPIO_PIN_RESET)
	{
		SetServoMotorExEvent(3);
	}
	 if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13)==GPIO_PIN_RESET)
	{
		SetServoMotorExEvent(4);
	}
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
	
	

	
	sMotor[0].event.scanEvent=EventScan0;
	sMotor[1].event.scanEvent=EventScan1;
	sMotor[2].event.scanEvent=EventScan2;	
	
	SMotorTask_Static.sMotorCount=SERVOMOTOR_NUM;
	SMotorTask_Static.sMotorList=sMotor;
	SMotorTask_Static.sMotorMonitorStep=0;
	
	
	  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	
	  HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	
	  HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	
	  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	  
	  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	  
	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	  

	
	ServoMotorTaskInit();
	
	
}
  


