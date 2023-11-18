#include "motor_cfg.h"

extern void motor_zero_touch(MOTOR* ths);
extern void motor_position_reach(MOTOR* ths);
extern void motor_move_test(u8 idx, f32 distance, u8 is_rel);
extern void motor_reset_test(u8 idx);
extern void motor_cordinate_update(u8 idx);


motor_cfg_struct motors_cfg[MOTOR_CNT]=
{
	{
		. zero_trig_vol	 = 0,		    //零位触发电压
		. dir_vol = 1,
		. enb_vol = 0,
		. half_vol = 0,
		. enable_zero_makeup	 = 0,			//是不是旋转电机
		.back_zero_speed_mode = 0,
		.subdivision=16,
		.currentLevel=15,
        .holdLevel = 6
	}
	
};		

DRIVER_REG motors_reg[MOTOR_CNT]=
{
	{
        .CHOPCONF=0x101D5,
        .COOLCONF=1<<24|0<<16 | 10<<8 | 0,
        .TPWMTHRS=20,
        .TCOOLTHRS=120,
        .THIGH=0,
        .VDCMIN=500<<8,
        .PWMCONF=0x20C04C8,
    }
};	

MOTOR motors[MOTOR_CNT];

Motor_cfg_extern_t motors_extern_cfgs[MOTOR_CNT] = {
    {
        .step_ratio = 5.468,
		.zero = 4.5,
        .max = 80,
        .lost_stop = 1,
        .lost_continue = 1,
		
		.swn_port = GPIOD,
        .swn_pin = GPIO_PIN_3,
		
        .swp_port = GPIOD,
        .swp_pin = GPIO_PIN_2,

        .rl_port = GPIOD,
        .rl_pin = GPIO_PIN_1,

        .rr_port = GPIOD,
        .rr_pin = GPIO_PIN_0,
		
        .cs_port = GPIOC,
        .cs_pin = GPIO_PIN_12,
		
        .en_port = GPIOC,
        .en_pin = GPIO_PIN_11,
		
        .em_port = GPIOC,
        .em_pin = GPIO_PIN_10,

        .zero_port = GPIOB,
        .zero_pin = GPIO_PIN_5,
    }
};

MotorThread MotorThread_OS[MOTOR_THEARD_CNT];

//队列成员应该是命令码和send对象	
SeqQueue_Struct   queue[MOTOR_THEARD_CNT];


CREATE_Q_BLOCK(0);


MOTOR_STATE_MACHINE(0);


MOTOR_TMC_RESET(0);						

			
MOTOR_TMC_BACKZERO(0);


MOTOR_TMC_LISTEN_IO(0);


MOTOR_TMC_LISTEN_EVENT(0);



//cs en em REFL_X  REFR_X
MOTOR_IO_WR_DECLARE(0,
        motors_extern_cfgs[0].cs_port, motors_extern_cfgs[0].cs_pin,
        motors_extern_cfgs[0].en_port, motors_extern_cfgs[0].en_pin,
        motors_extern_cfgs[0].em_port, motors_extern_cfgs[0].em_pin,
        motors_extern_cfgs[0].rl_port, motors_extern_cfgs[0].rl_pin,
        motors_extern_cfgs[0].rr_port, motors_extern_cfgs[0].rr_pin
    );
	

//光耦接触到时IO中断  比较位置寄存器IO中断		SWN_X
MOTOR_IO_RD_DECLARE(0,
        motors_extern_cfgs[0].zero_port, motors_extern_cfgs[0].zero_pin,
        motors_extern_cfgs[0].swp_port, motors_extern_cfgs[0].swp_pin,
        motors_extern_cfgs[0].swn_port, motors_extern_cfgs[0].swn_pin
    );

u8 m0_event_read(u8 event_id){
return !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6);
}


void motor_move_done_notify(u8 idx){ }
	
void MotorInit(void)
{
	MOTOR_TMC_INIT(0);//电机控制器初始化

	
    motor_cordinate_update(0);//电机坐标更新


	
	MOTOR_THRAD_CREATE(6, 0);//输入电机工作线程的优先级以及对应的电机编号并生成， 信号线 以及一个队列



#if 0


	motor_reset_test(0);
//	OSTimeDly(1000);
	
   //motor_move_test(0, 1000, 1);

#endif
}

/*
电机中断配置表
M1:
zero:PD15;swp:PC7


*/


//void EXTI9_5_IRQHandler(void)//中断函数
//{
//	
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);

//}

//void EXTI2_IRQHandler(void)//中断函数
//{
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);

//}


//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	switch(GPIO_Pin)
//	{

//		case GPIO_PIN_2:
//			motor_position_reach(&motors[0]);
//			break;
//		case GPIO_PIN_5:
//		    motor_zero_touch(&motors[0]);
//			break;

//		default:break;
//	}		
//}
