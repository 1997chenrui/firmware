#include "bus.h"
#include "motor_thread.h"
#include "mTMC5130Device.h"
#include "itf_01000c10.h"
#include "itf_01000c40.h"
#include "io_mod.h"

extern TIM_HandleTypeDef htim2;
extern MOTOR motors[];
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);
u8 event_id = 0;

static u8 sensor_trig(){
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET;
}

static u8 check(u8 idx, f32 distance, f32 speed, f32 acce)
{
	motors[idx].id = idx;
	motors[idx].postion = motor_step_calc(idx, distance);
	motors[idx].speed = motor_step_calc(idx, speed);
	motors[idx].acc = motor_step_calc(idx, acce);
	motors[idx].isBackZero = 0;
	motors[idx].isRelative = 1;
	u8 error_code = motors[idx].mTMC5130MotorMove(&motors[idx]);
	return sensor_trig();
}

static u32 bottle_check(f32 x_speed, f32 x_acce, f32 y_speed, f32 y_acce)
{
	f32 x_distance = 45, x1_distance = 59, y_distance = 45;
	u32 state = 0;
	u8 idx = 0;
	state |= sensor_trig();
	idx++;
	//第一行
	for (int y = 0; y < 4; y++)
	{
		state |= (check(1, y_distance, y_speed, y_acce) << idx++);
	}
	//X向偏移
	state |= (check(0, x_distance, x_speed, x_acce) << idx++);
	y_distance = -y_distance;
	//第二行
	for (int y = 0; y < 4; y++)
	{
		state |= (check(1, y_distance, y_speed, y_acce) << idx++);
	}
	//X向偏移 此处偏移与结构相关
	state |= (check(0, x1_distance, x_speed, x_acce) << idx++);
	y_distance = -y_distance;
	//第三行
	for (int y = 0; y < 4; y++)
	{
		state |= (check(1, y_distance, y_speed, y_acce) << idx++);
	}
	//X向偏移
	state |= (check(0, x_distance, x_speed, x_acce) << idx++);
	y_distance = -y_distance;
	//第四行
	for (int y = 0; y < 4; y++)
	{
		state |= (check(1, y_distance, y_speed, y_acce) << idx++);
	}
	return state;
}

u8 motor_cmd_extend(u32 itf_code, bus_packet *packet)
{
	switch (itf_code)
	{
	case 0x01000c10:
	{
		Itf_01000c10_t *cmd = Itf_01000c10_new();
		cmd->packet_parse(cmd, packet);
		queue_ele_enter(0, itf_code, packet, cmd);
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
	case 0x01000c10:
	{
		Itf_01000c10_t *cmd = ele->p_cmd;
		u32 state = bottle_check(cmd->content.x_speed, cmd->content.x_acce,
								 cmd->content.y_speed, cmd->content.y_acce);
		cmd->rtn_and_free(cmd, &ele->packet_head, state);
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
	{
		motor_cmd_extend(func_code, packet);
		break;
	}
	default:
		return 1;
	}
	return 0;
}
