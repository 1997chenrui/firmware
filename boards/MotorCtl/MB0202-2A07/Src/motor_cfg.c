#include "motor_cfg.h"

extern void motor_zero_touch(MOTOR* ths);
extern void motor_position_reach(MOTOR* ths);
extern void motor_move_test(u8 idx, f32 distance, u8 is_rel);
extern void motor_reset_test(u8 idx);
extern void motor_cordinate_update(u8 idx);
void motor_rotate_test(u8 idx, u16 time_ms, u8 dir);

///<电机参数配置表
/*
0:
	玻片抓手Z电机M5
1：	
	玻片抓手Y电机M2
2:
	玻片抓手旋转电机M3
3：	
	玻片夹爪电机M4

*/
motor_cfg_struct motors_cfg[MOTOR_CNT]=
{
	{
		. zero_trig_vol	 = 0,		    //零位触发电压
		. dir_vol = 0,
		. enb_vol = 0,
		. half_vol = 0,
		. enable_zero_makeup	 = 0,			//是不是旋转电机
		.back_zero_speed_mode = 0,
		.subdivision=16,
		.currentLevel=30,
        .holdLevel = 15
	},
	{
		. zero_trig_vol	 = 0,		    //零位触发电压
		. dir_vol = 0,
		. enb_vol = 0,
		. half_vol = 0,
		. enable_zero_makeup	 = 0,			//是不是旋转电机
		.back_zero_speed_mode = 0,
		.subdivision=16,
		.currentLevel=15,
        .holdLevel = 5
	},
	{
		. zero_trig_vol	 = 0,		    //零位触发电压
		. dir_vol = 1,
		. enb_vol = 0,
		. half_vol = 0,
		. enable_zero_makeup	 = 0,			//是不是旋转电机
		.back_zero_speed_mode = 0,
		.subdivision=16,
		.currentLevel=15,
        .holdLevel = 5
	},
	{
		. zero_trig_vol	 = 1,		    //零位触发电压
		. dir_vol = 1,
		. enb_vol = 0,
		. half_vol = 0,
		. enable_zero_makeup	 = 0,			//是不是旋转电机
		.back_zero_speed_mode = 0,
		.subdivision=16,
		.currentLevel=25,
        .holdLevel = 8
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
        .PWMCONF=0x504c8,
    },
	{
        .CHOPCONF=0x101D5,
        .COOLCONF=1<<24|0<<16 | 10<<8 | 0,
        .TPWMTHRS=20,
        .TCOOLTHRS=120,
        .THIGH=0,
        .VDCMIN=500<<8,
        .PWMCONF=0x504c8,
    }
	,
	{
        .CHOPCONF=0x101D5,
        .COOLCONF=1<<24|0<<16 | 10<<8 | 0,
        .TPWMTHRS=20,
        .TCOOLTHRS=120,
        .THIGH=0,
        .VDCMIN=500<<8,
        .PWMCONF=0x504c8,
    }
	,
	{
        .CHOPCONF=0x101D5,
        .COOLCONF=1<<24|0<<16 | 10<<8 | 0,
        .TPWMTHRS=20,
        .TCOOLTHRS=120,
        .THIGH=0,
        .VDCMIN=500<<8,
        .PWMCONF=0x504c8,
    }
};	

MOTOR motors[MOTOR_CNT];

Motor_cfg_extern_t motors_extern_cfgs[MOTOR_CNT] = {
	 {
        .step_ratio = 5.468066492,
		.zero = 4.5,
        .max = 75.5,//-54.5
        .lost_stop = 0.5,
        .lost_continue = 0.5,
        .cs_port = GPIOE,
        .cs_pin = GPIO_PIN_13,
        .en_port = GPIOE,
        .en_pin = GPIO_PIN_14,
        .em_port = GPIOE,
        .em_pin = GPIO_PIN_15,
        .rl_port = GPIOE,
        .rl_pin = GPIO_PIN_9,
        .rr_port = GPIOE,
        .rr_pin = GPIO_PIN_12,
        .swp_port = GPIOE,
        .swp_pin = GPIO_PIN_11,
        .swn_port = GPIOE,
        .swn_pin = GPIO_PIN_10,
        .zero_port = GPIOB,
        .zero_pin = GPIO_PIN_12,
    },
	  {
        .step_ratio = 5.468066492,
		.zero = 4,
        .max = 194,
        .lost_stop = 1,
        .lost_continue = 1,
        .cs_port = GPIOD,
        .cs_pin = GPIO_PIN_4,
        .en_port = GPIOD,
        .en_pin = GPIO_PIN_5,
        .em_port = GPIOD,
        .em_pin = GPIO_PIN_6,
        .rl_port = GPIOD,
        .rl_pin = GPIO_PIN_0,
        .rr_port = GPIOD,
        .rr_pin = GPIO_PIN_1,
        .swp_port = GPIOD,
        .swp_pin = GPIO_PIN_3,
        .swn_port = GPIOD,
        .swn_pin = GPIO_PIN_2,
        .zero_port = GPIOB,
        .zero_pin = GPIO_PIN_4,
    },
    {
        .step_ratio = 1.388888889,
        .zero = 1,
        .max = 87.43,//-87.82
        .lost_stop = 1,
        .lost_continue = 1,
        .cs_port = GPIOD,
        .cs_pin = GPIO_PIN_12,
        .en_port = GPIOD,
        .en_pin = GPIO_PIN_13,
        .em_port = GPIOD,
        .em_pin = GPIO_PIN_14,
        .rl_port = GPIOD,
        .rl_pin = GPIO_PIN_10,
        .rr_port = GPIOD,
        .rr_pin = GPIO_PIN_11,
        .swp_port = GPIOD,
        .swp_pin = GPIO_PIN_9,
        .swn_port = GPIOD,
        .swn_pin = GPIO_PIN_8,
        .zero_port = GPIOB,
        .zero_pin = GPIO_PIN_14,
    },
    {
        .step_ratio = 153.8461538,
		.zero = 0.5,
        .max = 5,
        .lost_stop = 0.3,
        .lost_continue = 0.3,
        .cs_port = GPIOC,
        .cs_pin = GPIO_PIN_10,
        .en_port = GPIOC,
        .en_pin = GPIO_PIN_11,
        .em_port = GPIOC,
        .em_pin = GPIO_PIN_12,
        .rl_port = GPIOC,
        .rl_pin = GPIO_PIN_8,
        .rr_port = GPIOC,
        .rr_pin = GPIO_PIN_9,
        .swp_port = GPIOC,
        .swp_pin = GPIO_PIN_7,
        .swn_port = GPIOC,
        .swn_pin = GPIO_PIN_6,
        .zero_port = GPIOD,
        .zero_pin = GPIO_PIN_15,
    }
};

MotorThread MotorThread_OS[MOTOR_THEARD_CNT];

//队列成员应该是命令码和send对象	
SeqQueue_Struct   queue[MOTOR_THEARD_CNT];


CREATE_Q_BLOCK(0);
CREATE_Q_BLOCK(1);
CREATE_Q_BLOCK(2);
CREATE_Q_BLOCK(3);


MOTOR_STATE_MACHINE(0);
MOTOR_STATE_MACHINE(1);
MOTOR_STATE_MACHINE(2);
MOTOR_STATE_MACHINE(3);

MOTOR_TMC_RESET(0);						
MOTOR_TMC_RESET(1);
MOTOR_TMC_RESET(2);	
MOTOR_TMC_RESET(3);	
			
MOTOR_TMC_BACKZERO(0);
MOTOR_TMC_BACKZERO(1);
MOTOR_TMC_BACKZERO(2);
MOTOR_TMC_BACKZERO(3);

MOTOR_TMC_LISTEN_IO(0);
MOTOR_TMC_LISTEN_IO(1);
MOTOR_TMC_LISTEN_IO(2);
MOTOR_TMC_LISTEN_IO(3);

MOTOR_TMC_LISTEN_EVENT(0);
MOTOR_TMC_LISTEN_EVENT(1);
MOTOR_TMC_LISTEN_EVENT(2);
MOTOR_TMC_LISTEN_EVENT(3);

//cs en em REFL_X  REFR_X
MOTOR_IO_WR_DECLARE(0,
        motors_extern_cfgs[0].cs_port, motors_extern_cfgs[0].cs_pin,
        motors_extern_cfgs[0].en_port, motors_extern_cfgs[0].en_pin,
        motors_extern_cfgs[0].em_port, motors_extern_cfgs[0].em_pin,
        motors_extern_cfgs[0].rl_port, motors_extern_cfgs[0].rl_pin,
        motors_extern_cfgs[0].rr_port, motors_extern_cfgs[0].rr_pin
    );
MOTOR_IO_WR_DECLARE(1,
        motors_extern_cfgs[1].cs_port, motors_extern_cfgs[1].cs_pin,
        motors_extern_cfgs[1].en_port, motors_extern_cfgs[1].en_pin,
        motors_extern_cfgs[1].em_port, motors_extern_cfgs[1].em_pin,
        motors_extern_cfgs[1].rl_port, motors_extern_cfgs[1].rl_pin,
        motors_extern_cfgs[1].rr_port, motors_extern_cfgs[1].rr_pin
    );
MOTOR_IO_WR_DECLARE(2,
        motors_extern_cfgs[2].cs_port, motors_extern_cfgs[2].cs_pin,
        motors_extern_cfgs[2].en_port, motors_extern_cfgs[2].en_pin,
        motors_extern_cfgs[2].em_port, motors_extern_cfgs[2].em_pin,
        motors_extern_cfgs[2].rl_port, motors_extern_cfgs[2].rl_pin,
        motors_extern_cfgs[2].rr_port, motors_extern_cfgs[2].rr_pin
    );
MOTOR_IO_WR_DECLARE(3,
        motors_extern_cfgs[3].cs_port, motors_extern_cfgs[3].cs_pin,
        motors_extern_cfgs[3].en_port, motors_extern_cfgs[3].en_pin,
        motors_extern_cfgs[3].em_port, motors_extern_cfgs[3].em_pin,
        motors_extern_cfgs[3].rl_port, motors_extern_cfgs[3].rl_pin,
        motors_extern_cfgs[3].rr_port, motors_extern_cfgs[3].rr_pin
    );

//光耦接触到时IO中断  比较位置寄存器IO中断		SWN_X
MOTOR_IO_RD_DECLARE(0,
        motors_extern_cfgs[0].zero_port, motors_extern_cfgs[0].zero_pin,
        motors_extern_cfgs[0].swp_port, motors_extern_cfgs[0].swp_pin,
        motors_extern_cfgs[0].swn_port, motors_extern_cfgs[0].swn_pin
    );
MOTOR_IO_RD_DECLARE(1,
        motors_extern_cfgs[1].zero_port, motors_extern_cfgs[1].zero_pin,
        motors_extern_cfgs[1].swp_port, motors_extern_cfgs[1].swp_pin,
        motors_extern_cfgs[1].swn_port, motors_extern_cfgs[1].swn_pin
    );
MOTOR_IO_RD_DECLARE(2,
        motors_extern_cfgs[2].zero_port, motors_extern_cfgs[2].zero_pin,
        motors_extern_cfgs[2].swp_port, motors_extern_cfgs[2].swp_pin,
        motors_extern_cfgs[2].swn_port, motors_extern_cfgs[2].swn_pin
    );
MOTOR_IO_RD_DECLARE(3,
        motors_extern_cfgs[3].zero_port, motors_extern_cfgs[3].zero_pin,
        motors_extern_cfgs[3].swp_port, motors_extern_cfgs[3].swp_pin,
        motors_extern_cfgs[3].swn_port, motors_extern_cfgs[3].swn_pin
    );

u8 m0_event_read(u8 event_id){return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET;}
u8 m1_event_read(u8 event_id){return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET;}
u8 m2_event_read(u8 event_id){return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET;}
u8 m3_event_read(u8 event_id){return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET;}

void motor_move_done_notify(u8 idx){ }
	
void MotorInit(void)
{
	MOTOR_TMC_INIT(0);//电机控制器初始化
	MOTOR_TMC_INIT(1);
	MOTOR_TMC_INIT(2);
	MOTOR_TMC_INIT(3);
	
    motor_cordinate_update(0);//电机坐标更新
    motor_cordinate_update(1);
	motor_cordinate_update(2);
    motor_cordinate_update(3);
	
	MOTOR_THRAD_CREATE(6, 0);//输入电机工作线程的优先级以及对应的电机编号并生成， 信号线 以及一个队列
	MOTOR_THRAD_CREATE(7, 1);
	MOTOR_THRAD_CREATE(8, 2);
	MOTOR_THRAD_CREATE(10, 3);
	
#if 0
//  motor_reset_test(0);
//	motor_reset_test(1);
	motor_reset_test(2);
//	motor_reset_test(3);
//	motor_move_test(0, 10, 1);
//	motor_move_test(1, 10, 1);
//	motor_move_test(2, 10, 1);
//	motor_move_test(3, 10, 1);
  
#endif
}

void EXTI1_IRQHandler(void)//中断函数
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI3_IRQHandler(void)//中断函数
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void)//中断函数
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void)//中断函数
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)//中断函数
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case GPIO_PIN_11:
		{
			motor_position_reach(&motors[0]);					
		}break;
		case GPIO_PIN_12://IMPURT MOTOR
		{			
			motor_zero_touch(&motors[0]);					
		}break;
		
		case GPIO_PIN_3:  //OUT MOTOR
		{
			motor_position_reach(&motors[1]);					
		}break;
		case GPIO_PIN_4:
		{
            motor_zero_touch(&motors[1]);
		}break;
		
		case GPIO_PIN_9:
		{
			motor_position_reach(&motors[2]);					
		}break;
		case GPIO_PIN_14://IMPURT MOTOR
		{			
			motor_zero_touch(&motors[2]);					
		}break;	
		
		case GPIO_PIN_7:
		{
			motor_position_reach(&motors[3]);					
		}break;
		case GPIO_PIN_15://IMPURT MOTOR
		{			
			motor_zero_touch(&motors[3]);					
		}break;	
	}		
}

