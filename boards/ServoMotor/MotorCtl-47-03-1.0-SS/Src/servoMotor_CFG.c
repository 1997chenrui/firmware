#include "servoMotor_CFG.h"
#include "servoMotor.h"
#include "my_main.h"

#define DEBUG_LOG_DEBUG(s, args...) printf(s, ##args)
#define DEBUG_LOG_INFO(s, args...) printf(s, ##args)
#define DEBUG_LOG_WARN(s, args...)
#define DEBUG_LOG_ERROR(s, args...)

// 驱动使用到的硬件SPI
extern SPI_HandleTypeDef hspi1;
extern void delayUs(u16 us);

// 驱动器列表
TMC4361TypeDef tmc4361List[SERVOMOTOR_NUM];

// 伺服电机列表
SMotor sMotor[SERVOMOTOR_NUM];

/*
*********************************************************************************************************
*                                    先配置TMC4361驱动器
*********************************************************************************************************
*/

// 实现spi
void SpiWR_0(uint8_t *data)
{

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
	delayUs(20);
	CPU_SR_ALLOC();
	OS_ENTER_CRITICAL();
	HAL_SPI_TransmitReceive(&hspi1, data, data, 5, 0xFFFF);
	OS_EXIT_CRITICAL();
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
	delayUs(20);
}

// 电机紧急制动配置
void TMC4361A_NFREEZE_0(void)
{

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
	delayUs(20);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
}

void EXTI9_5_IRQHandler(void) // 中断函数
{

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)
{

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11); // 电机0的到达
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10); // 电机0的事件

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12); // 电机0的零位

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
}

extern void motor_zero_touch(MOTOR *ths);
extern void motor_position_reach(MOTOR *ths);
extern void motor_move_test(u8 idx, f32 distance, u8 is_rel);
extern void motor_reset_test(u8 idx);
extern void motor_cordinate_update(u8 idx);

#include "motor_cfg.h"

Add_Servomotor_Flag(0);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case GPIO_PIN_11: // 电机0到达
		Touch_Servomotor_Target(0);
		break;
	case GPIO_PIN_10: // 电机0中断事件
		Touch_Servomotor_Event(0);
		break;
	case GPIO_PIN_12: // 电机0的复位开关事件
		Touch_Servomotor_Zero(0);
		break;

	// 普通电机
	case GPIO_PIN_9:
		motor_position_reach(&motors[0]);
		break;
	case GPIO_PIN_14:
		motor_zero_touch(&motors[0]);
		break;

	default:
		break;
	}
}

// 伺服电机驱动器配置
TMC4361TypeDef tmc4361List[SERVOMOTOR_NUM] =
	{
		{.spi.spiWR = SpiWR_0,
		 .TMC4361A_NFREEZE = TMC4361A_NFREEZE_0}};

const TMC4361CFG tmc4361cfgList[SERVOMOTOR_NUM] =
	{
		{
			.setp_fs_per_rev = 200,			 // 电机旋转一周需要多少整步
			.setp_per_fs = 0,				 // 电机细分 0x00=256
			.ramp_reverse_motor_dir = false, // 是否改变电机方向
			.enc_in_invert_dir = true,		 // 是否反向编码器方向
			.enc_in_res = 4096,				 // 编码器的每圈步进数  0x4040000|
			.event_intr_conf = 0,			 // TMC4361A_CL_FIT_MASK|TMC4361A_CL_MAX_MASK,//开启的中断事件
			.cl_imin = 10,					 // 闭环最小电流调节值
			.cl_imax = 200					 // 闭环最大电流调节值

		}};

const TMC4361A_REARDRIVE_REG rearDriveReg[SERVOMOTOR_NUM][12] =
	{
		{{.adder = 0x00, .data = 0x12200},
		 {.adder = 0x10, .data = 0x00031905},					   // 驱动电流等
		 {.adder = 0x11, .data = 0x0000000A},					   // TZEROWAIT
		 {.adder = 0x13, .data = 20},							   // TPWMTHRS
		 {.adder = 0x14, .data = 120},							   // TCOOLTHRS
		 {.adder = 0x15, .data = 0},							   // THIGH
		 {.adder = 0x33, .data = 0},							   // VDCMIN
		 {.adder = 0x6c, .data = 0x00010223},					   // 斩波器和驱动器配置 CHOPCONF
		 {.adder = 0x6d, .data = 1 << 24 | 0 << 16 | 10 << 8 | 0}, // COOLCONF
		 {.adder = 0x6E, .data = 0x00070025},					   // DCCTRL
		 {.adder = 0x70, .data = 0x000404FF},					   // PWMCONF
		 {.adder = 0xFF, .data = 0x00}}};

// 驱动器初始化
static void TMC4361A_CFG_INIT()
{
	// 外部中断配置
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// 位置到达中断
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// 事件中断
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// 复位开关中断
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	for (uint8_t i = 0; i < SERVOMOTOR_NUM; i++)
	{
		tmc4361List[i].cfg = &tmc4361cfgList[i];
		tmc4361List[i].rear_reg = &rearDriveReg[i][0];
		TMC4361_SoftRest(&tmc4361List[i]);
		osDelay(200);
		TMC4361A_CFG_Init(&tmc4361List[i]);
	}
}

// 伺服电机配置
SMotorCfg sMotorCfgList[SERVOMOTOR_NUM] ={
	{	.restEventPol = 0,
		.defaultDir = !0,
		.step_ratio = 5.468,
		.zero_distance = -4.5,
		.max_distance = 317,
		.max_distance_reverse = 1
	}
};

bool TMC4361A_ZeroSwitchStateGet_0(void)
{
	return (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12));
}

void EventScan0(uint32_t *exflag, uint8_t *inflag)
{

	Update_Servomotor_Target(0);
	Update_Servomotor_Event(0);
	Update_Servomotor_Zero(0);

	Update_Servomotor_UserEvent(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET, 3);
}
void ServoMotorInit(void)
{
	TMC4361A_CFG_INIT();

	for (uint8_t i = 0; i < SERVOMOTOR_NUM; i++)
	{
		sMotor[i].drive = &tmc4361List[i];
		sMotor[i].cfg = &sMotorCfgList[i];
	}

	sMotor[0].sMotorGetRestSwitchState = TMC4361A_ZeroSwitchStateGet_0;

	SMotorTask_Static.sMotorCount = SERVOMOTOR_NUM;
	SMotorTask_Static.sMotorList = sMotor;

	SMotorTask_Static.sMotorMonitorStep = 0;

	sMotor[0].event.scanEvent = EventScan0;

	SMotorTask_Static.extend.tmc5130MotorCount = MOTOR_CNT;
	SMotorTask_Static.extend.tmc5130MotorList = motors;

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	ServoMotorTaskInit();
}
