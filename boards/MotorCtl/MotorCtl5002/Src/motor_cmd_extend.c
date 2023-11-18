#include "bus.h"
#include "motor_thread.h"
#include "motor_cfg.h"
#include "mTMC5130Device.h"
#include "itf_01000910.h"
#include "io_mod.h"
#include "voltage.h"

extern MOTOR motors[];
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);
extern void free_seq_args(int cnt, motor_seq_move_args_t* seqArgs);

static u8 wait_motor_seq_move(u8 mix_percent, u16 mix_ms, u16 timeout_ms){
    //开始时间
    u8 time_span = 5;
    u16 cnt = timeout_ms / time_span;
    u8 trigger = 0;
    for(int i = 0; i < cnt; i++) {
        if(trigger > 2){
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);//发同步信号给检测板（拉高位有效）
			Voltage_t * voltage = voltage_get_as_ref(0);
			voltage->voltage_write_persent_time(voltage, mix_percent, mix_ms);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);//发同步信号给检测板（拉低为无效）
            return 0;
        }
        if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0)){
            trigger++;
        }
        bus_os_sleep(time_span);
    }
    return 1;
}

u8 motor_cmd_extend(u32 itf_code, bus_packet *packet)
{
	switch (itf_code)
	{
	case 0x01000910:
	{
		Itf_01000910_t* cmd = Itf_01000910_new();
		cmd->packet_parse(cmd, packet);
		queue_ele_enter(cmd->content.seq_array[0].args[0].idx_rel.data_u.idx, itf_code, packet, cmd);
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
	case 0x01000910:
	{
		motor_cmd_extend(func_code, packet);
		break;
	}
	default:
		return 1;
	}
	return 0;
}

u8 motor_thread_exe_extend(Motor_queue_ele_t * ele){
	switch(ele->cmd_code){
		case 0x01000910:
		{
			printf("0x01000910\r\n");
			Itf_01000910_t *p_cmd = ele->p_cmd;
			motor_seq_move_args_t* seqArgs = malloc(sizeof(motor_seq_move_args_t)*p_cmd->content.seq_len);
			memset(seqArgs, 0, sizeof(motor_seq_move_args_t)*p_cmd->content.seq_len);
			for(int i = 0; i < p_cmd->content.seq_len; i++)
			{
				motor_seq_move_args_t* seq = &seqArgs[i];
				seq->args_len = p_cmd->content.seq_array[i].arg_len;
				seq->args = (motor_multi_move_args_t*)malloc(sizeof(motor_multi_move_args_t)*seq->args_len);
				memset(seq->args, 0, sizeof(motor_multi_move_args_t)*seq->args_len);
				seq->m2_delayms = p_cmd->content.seq_array[i].delay_ms;
				for(int j = 0; j < seq->args_len;j++)
				{
					u8 idx = p_cmd->content.seq_array[i].args[j].idx_rel.data_u.idx;
					motor_multi_move_args_t* multi = &seq->args[j];
					multi->motor=&motors[idx];
					multi->m1_idx=idx;
					multi->m2_acce=motor_step_calc(idx, p_cmd->content.seq_array[i].args[j].acce);
					multi->m3_speed=motor_step_calc(idx, p_cmd->content.seq_array[i].args[j].speed);
					multi->m4_distance=motor_step_calc(idx, p_cmd->content.seq_array[i].args[j].distance);
					multi->m5_isRelative=p_cmd->content.seq_array[i].args[j].idx_rel.data_u.is_rel;
					multi->motor->id=idx;
				}
			}
			u8 error_code = 0;
			if(wait_motor_seq_move(p_cmd->content.mix_percent, p_cmd->content.mix_ms, p_cmd->content.timeout_ms) == 0){
				error_code = seqArgs[0].args[0].motor->mTMC5130Motor_SeqMove(p_cmd->content.seq_len, seqArgs);
			}else{
				error_code = MOTOR_STATUS_ERROR_TIME_OUT;
			}
			free_seq_args(p_cmd->content.seq_len, seqArgs);
			p_cmd->rtn_and_free(p_cmd, &ele->packet_head, error_code);
			break;
		} 
		default: 
			return 1;
	}
    return 0;
}
