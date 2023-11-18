#include "bus.h"
#include "motor_thread.h"
#include "mTMC5130Device.h"
#include "itf_01000c40.h"
#include "io_mod.h"

#define TIM_CLEAR()                                \
	do                                             \
	{                                              \
		__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE); \
	} while (0)
#define TIM_CNT_SET(v) \
	do                 \
	{                  \
		TIM2->ARR = v; \
	} while (0)
#define TIM_START()                    \
	do                                 \
	{                                  \
		TIM2->CNT = 0;                 \
		TIM_CLEAR();                   \
		HAL_TIM_Base_Start_IT(&htim2); \
	} while (0)
#define TIM_STOP()                 \
	do                             \
	{                              \
		HAL_TIM_Base_Stop(&htim2); \
	} while (0)
#define TIME_SPAN 5000

extern TIM_HandleTypeDef htim2;
extern MOTOR motors[];
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);
u8 event_id = 0;

void TIM2_IRQHandler(void)
{
	TIM_CLEAR();
	// 5ms
	TIM_CNT_SET(TIME_SPAN);
	if (event_id == 0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6));
	}
	else if(event_id == 1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7));
	}
	else
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7));
	}
}

static void calibrate_cap_ctl(Itf_01000c40_content_t content)
{
	if (content.idx_state.split.enable)
	{
		TIM_CNT_SET(TIME_SPAN);
		TIM_START();
		event_id = content.idx_state.split.idx;
	}
	else
	{
		TIM_STOP();
	}
}

u8 motor_cmd_extend(u32 itf_code, bus_packet *packet)
{
	switch (itf_code)
	{
	case 0x01000c40:
	{
		Itf_01000c40_t cmd;
		itf_01000c40_init(&cmd);
		Itf_01000c40_content_t content = cmd.packet_parse(packet);
		calibrate_cap_ctl(content);
		cmd.rtn(packet, 0);
		break;
	}
	default:
		return 1;
	}
	return 0;
}

u8 bus_ita_func_callback_extern(u32 func_code, bus_packet *packet)
{
	switch (func_code)
	{
	case 0x01000c40:
	{
		motor_cmd_run(func_code, packet);
		break;
	}
	default:
		return 1;
	}
	return 0;
}
