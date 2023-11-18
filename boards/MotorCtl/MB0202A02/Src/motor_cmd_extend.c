#include "bus.h"
#include "motor_thread.h"
#include "mTMC5130Device.h"
#include "itf_01000c11.h"
#include "itf_01000c12.h"
#include "itf_01000c40.h"
#include "io_mod.h"
#include "listenandsync_cfg.h"
#define SLIDE_CNT 900
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern MOTOR motors[];
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);
extern f32 motor_distance_calc(u8 idx, int value);
extern MOTOR motors[];
u8 event_id = 0;
static u16 slide_cnt;
static f32 slide_position[SLIDE_CNT];

static u8 timer_cicle_cnt;
static u32 timer_cnt;
static f32 circle_scope;

static u8 slide_trig(){
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_RESET;
}

static u8 check(u8 idx, f32 distance, f32 speed, f32 acce)//Y电机运动
{
	motors[idx].id = idx;
	motors[idx].postion = motor_step_calc(idx, distance);
	motors[idx].speed = motor_step_calc(idx, speed);
	motors[idx].acc = motor_step_calc(idx, acce);
	motors[idx].isBackZero = 0;
	motors[idx].isRelative = 1;
	u8 error_code = motors[idx].mTMC5130MotorMove(&motors[idx]);
	return slide_trig();
}

static u32 slide_check(u8 idx,f32 y_distance,f32 y_speed, f32 y_acce)//Y电机移动
{
	motors[idx].id = idx;
	motors[idx].postion = motor_step_calc(idx, y_distance);
	motors[idx].speed = motor_step_calc(idx, y_speed);
	motors[idx].acc = motor_step_calc(idx, y_acce);
	motors[idx].isBackZero = 0;
	motors[idx].isRelative = 1;
	u8 error_code = motors[idx].mTMC5130MotorMove(&motors[idx]);
	return error_code;
}

/*
	定时器中断（1ms一次）判断玻片传感器是否检测到跳变信号，若检测到，则将电机位置保存在数组中，后面再传给上位机
*/
void TIM3_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
	timer_cnt++;
	if(timer_cnt == timer_cicle_cnt)
	{
		timer_cnt = 0;
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_RESET)
		{
			slide_position[slide_cnt] = motor_distance_calc(1, mTMC5130DeviceGetPostion(&motors[1],1));//记录电机的位置
			slide_cnt++;
			if(slide_cnt >= (SLIDE_CNT-100))
			{
				HAL_TIM_Base_Stop_IT(&htim3);
			}
		}
	}	
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

static u32 bottle_check(f32 y_speed, f32 y_acce)
{
	f32  y_distance = 3.6;
	u16 state = 0;
	u8 idx = 0;
	state |= slide_trig();
	idx++;
	
	for (int y = 0; y < 10; y++)
	{
		state |= (check(1, y_distance, y_speed, y_acce) << idx++);//用每一位二进制位来记录玻片状态
	}
	return state;
}

static void calibrate_cap_ctl(Itf_01000c40_content_t content)
{
	if (content.idx_state.split.enable)
	{
		//TIM_CNT_SET(TIME_SPAN);
		//TIM_START();
		listenAndSync_config.listenList[0].Start(&listenAndSync_config.listenList[0]);
	}
	else
	{
		listenAndSync_config.listenList[0].Stop(&listenAndSync_config.listenList[0]);
	}
}

u8 motor_cmd_extend(u32 itf_code, bus_packet *packet)
{
	switch (itf_code)
	{
	case 0x01000c11:
	{
		Itf_01000c11_t *cmd = Itf_01000c11_new();
		cmd->packet_parse(cmd, packet);
		queue_ele_enter(0, itf_code, packet, cmd);
		break;
	}
	case 0x01000c12:
	{
		Itf_01000c12_t *cmd = Itf_01000c12_new();
		cmd->packet_parse(cmd, packet);
		queue_ele_enter(0, itf_code, packet, cmd);
		break;
	}
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

u8 motor_thread_exe_extend(Motor_queue_ele_t *ele)
{
	switch (ele->cmd_code)
	{
	case 0x01000c11:
	{
		Itf_01000c11_t *cmd = ele->p_cmd;
		u32 state = bottle_check(cmd->content.y_speed, cmd->content.y_acce);
		cmd->rtn_and_free(cmd, &ele->packet_head, state);
		break;
	}
	case 0x01000c12:
	{
		Itf_01000c12_t *cmd = ele->p_cmd;
		slide_cnt = 0;
		timer_cnt = 0;
		circle_scope = (1.0*1000)/(cmd->content.m1_speed*3);
		timer_cicle_cnt = (u8)(circle_scope);
		HAL_TIM_Base_Start_IT(&htim3);
		slide_check(1,cmd->content.m3_target_distance,cmd->content.m1_speed,cmd->content.m2_acce);
		HAL_TIM_Base_Stop_IT(&htim3);
		for(int i=0;i<slide_cnt;i++)
		{
			printf("%f ",slide_position[i]);
		}
		printf("slide_cnt = %d\r\n",slide_cnt);
		cmd->rtn_and_free(cmd, &ele->packet_head, slide_position,slide_cnt);
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
	case 0x01000c10:
	case 0x01000c11:
	case 0x01000c12:
	case 0x01000c40:
	{
		motor_cmd_extend(func_code, packet);
		break;
	}
	default:
		return 1;
	}
	return 0;
}
