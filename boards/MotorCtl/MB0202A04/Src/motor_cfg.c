#include "motor_cfg.h"

extern void motor_zero_touch(MOTOR* ths);
extern void motor_position_reach(MOTOR* ths);
extern void motor_move_test(u8 idx, f32 distance, u8 is_rel);
extern void motor_reset_test(u8 idx);
extern void motor_cordinate_update(u8 idx);

///<电机参数配置表
/*
0:
	1号染色池吸取柱塞泵电机M5
1：	
	2号染色池吸取柱塞泵电机M3
2:
	抽负压柱塞泵电机M4


*/
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
		.currentLevel=25,
        .holdLevel = 13
	},
	{
		. zero_trig_vol	 = 0,		    //零位触发电压
		. dir_vol = 1,
		. enb_vol = 0,
		. half_vol = 0,
		. enable_zero_makeup	 = 0,			//是不是旋转电机
		.back_zero_speed_mode = 0,
		.subdivision=16,
		.currentLevel=25,
        .holdLevel = 13
	},
	{
		. zero_trig_vol	 = 0,		    //零位触发电压
		. dir_vol = 1,
		. enb_vol = 0,
		. half_vol = 0,
		. enable_zero_makeup	 = 0,			//是不是旋转电机
		.back_zero_speed_mode = 0,
		.subdivision=16,
		.currentLevel=25,
        .holdLevel = 13
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
};	

MOTOR motors[MOTOR_CNT];

Motor_cfg_extern_t motors_extern_cfgs[MOTOR_CNT] = {
	 {
        .step_ratio = 17.005872,
        .zero = 1.85,
        .max = 41.01,
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
        .step_ratio = 17.005872,
        .zero = 1.85,
        .max = 41.01,
        .lost_stop = 0.5,
        .lost_continue = 0.5,
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
        .step_ratio = 22.2,
		.zero = 1.85,
        .max = 41.01,
        .lost_stop = 0.5,
        .lost_continue = 0.5,
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




MOTOR_STATE_MACHINE(0);
MOTOR_STATE_MACHINE(1);
MOTOR_STATE_MACHINE(2);



MOTOR_TMC_RESET(0);						
MOTOR_TMC_RESET(1);
MOTOR_TMC_RESET(2);	


			
MOTOR_TMC_BACKZERO(0);
MOTOR_TMC_BACKZERO(1);
MOTOR_TMC_BACKZERO(2);


MOTOR_TMC_LISTEN_IO(0);
MOTOR_TMC_LISTEN_IO(1);
MOTOR_TMC_LISTEN_IO(2);


MOTOR_TMC_LISTEN_EVENT(0);
MOTOR_TMC_LISTEN_EVENT(1);
MOTOR_TMC_LISTEN_EVENT(2);


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


__weak u8 m0_event_read(u8 event_id){return 0;}
__weak u8 m1_event_read(u8 event_id){return 0;}
__weak u8 m2_event_read(u8 event_id){return 0;}



void motor_move_done_notify(u8 idx){ }
	
void MotorInit(void)
{
	OSTimeDlyHMSM(0,0,1,0);//延时1秒
	MOTOR_TMC_INIT(0);//电机控制器初始化
	MOTOR_TMC_INIT(1);
	MOTOR_TMC_INIT(2);


	
    motor_cordinate_update(0);//电机坐标更新
    motor_cordinate_update(1);
	motor_cordinate_update(2);


	
	MOTOR_THRAD_CREATE(6, 0);//输入电机工作线程的优先级以及对应的电机编号并生成， 信号线 以及一个队列
	MOTOR_THRAD_CREATE(7, 1);
	MOTOR_THRAD_CREATE(8, 2);


	
#if 0
	printf("motor test!\r\n");
    motor_reset_test(0);
	motor_reset_test(1);
	motor_reset_test(2);
#endif
}

void EXTI3_IRQHandler(void)//中断函数
{

}

void EXTI4_IRQHandler(void)//中断函数
{

}

void EXTI9_5_IRQHandler(void)//中断函数
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}

void EXTI15_10_IRQHandler(void)//中断函数
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
	
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
	 HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case GPIO_PIN_11:  //OUT MOTOR
		{
			motor_position_reach(&motors[0]);					
		}break;
		case GPIO_PIN_12:
		{
            motor_zero_touch(&motors[0]);
		}break;
		
		case GPIO_PIN_9:  //OUT MOTOR
		{
			motor_position_reach(&motors[1]);					
		}break;
		case GPIO_PIN_14:
		{
            motor_zero_touch(&motors[1]);
		}break;
		
		case GPIO_PIN_7:  //OUT MOTOR
		{
			motor_position_reach(&motors[2]);					
		}break;
		case GPIO_PIN_15:
		{
            motor_zero_touch(&motors[2]);
		}break;		
		
				
		default:break;
	}		
}

