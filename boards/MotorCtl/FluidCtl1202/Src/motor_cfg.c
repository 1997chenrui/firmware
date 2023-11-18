#include "motor_cfg.h"

extern void motor_zero_touch(MOTOR* ths);
extern void motor_position_reach(MOTOR* ths);
extern void motor_cordinate_update(u8 idx);
extern void motor_move_test(u8 idx, f32 distance, u8 is_rel);
extern void motor_reset_test(u8 idx);

///<电机参数配置表
motor_cfg_struct motors_cfg[MOTOR_CNT]=
{
	{
		. zero_trig_vol	 = 0,		    //零位触发电压
		. dir_vol = 0,
		. enb_vol = 0,
		. half_vol = 0,
		. enable_zero_makeup	 = 1,			//是不是旋转电机
		.back_zero_speed_mode = 0,
		.subdivision=16,
        .holdLevel = 1,
		.currentLevel=7,
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
    }
};	

MOTOR motors[MOTOR_CNT];

Motor_cfg_extern_t motors_extern_cfgs[MOTOR_CNT] = {
    {
        .step_ratio = 8,
        .zero = 2,
        .max = 250,
        .lost_stop = 4.6,
        .lost_continue = 4,
        .cs_port = GPIOE,
        .cs_pin = GPIO_PIN_11,
        .en_port = GPIOA,
        .en_pin = GPIO_PIN_4,
        .em_port = GPIOE,
        .em_pin = GPIO_PIN_9,
        .rl_port = GPIOE,
        .rl_pin = GPIO_PIN_8,
        .rr_port = GPIOE,
        .rr_pin = GPIO_PIN_10,
        .swp_port = GPIOB,
        .swp_pin = GPIO_PIN_10,
        .swn_port = GPIOE,
        .swn_pin = GPIO_PIN_12,
        .zero_port = GPIOE,
        .zero_pin = GPIO_PIN_15,
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

u8 m0_event_read(u8 event_id){return 0;}

void motor_move_done_notify(u8 idx){ 
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
    OSTimeDly(5);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
    OSTimeDly(15);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
}
	
void MotorInit(void)
{
	OSTimeDlyHMSM(0,0,1,0);
	MOTOR_TMC_INIT(0);
    motor_cordinate_update(0);
	MOTOR_THRAD_CREATE(6, 0);
#if 0
    motor_reset_test(0);
    motor_move_test(0, 125, 1);
#endif
}

void EXTI15_10_IRQHandler(void)//中断函数
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case GPIO_PIN_10:  //OUT MOTOR
		{
            motor_position_reach(&motors[0]);
		}break;
		case GPIO_PIN_15:
		{
            motor_zero_touch(&motors[0]);
		}break;
		default:break;
	}		
}

