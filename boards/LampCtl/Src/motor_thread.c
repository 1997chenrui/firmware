#include "motor_thread.h"
#include "Protocols.h"
#include "cmd.h"
#include "mTMC5130Device.h"
#include "bus.h"
#include "motor_cfg.h"
#include "itf_00010000.h"
#include "itf_00010010.h"
#include "itf_00010020.h"
#include "itf_00010030.h"
#include "itf_00010031.h"
#include "itf_00010040.h"
#include "itf_00010050.h"
#include "itf_00010060.h"
#include "itf_00010070.h"
#include "itf_00010080.h"
#include "itf_00010082.h"

extern MotorThread MotorThread_OS[];
//队列成员应该是命令码和send对象	
extern SeqQueue_Struct   queue[];

///<电机参数配置表
extern motor_cfg_struct motors_cfg[];

extern DRIVER_REG motors_reg[];	

extern MOTOR motors[];

extern Motor_cfg_extern_t motors_extern_cfgs[];

void motor_zero_touch(MOTOR* ths){
    if(ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,1) != ths->mot_cfg->zero_trig_vol) {
        ths->read_io(PIN_ID_SET_TOUCH_ISR,ths->zeor_isr_id,ths->mot_cfg->zero_trig_vol);
        return;
    }
    mTMC5130Motor_ZeroTouch(ths);
    mTMC5130ProcessSemPost(ths,WAIT_TOUCH_SEM);
}

void motor_position_reach(MOTOR* ths) {
    /*位置到达信号均为下降沿触发*/
    if(ths->read_io(PIN_ID_SET_REACH_SENSOR,ths->reach_isr_id,1) != 0) {
        ths->read_io(PIN_ID_SET_REACH_ISR,ths->reach_isr_id,0);
        return;
    }
    mTMC5130ProcessSemPost(ths,WAIT_REACH_SEM);
    mTMC5130ProcessSemPost(ths,WAIT_REACH_FLG);
}

void MotorThread_EnQueue(int idx,Motor_Queue_Ele_t *data)
{
    SeqQueue_Enter(&queue[idx],data);
}

void motor_cordinate_update(u8 idx) {
    ASSERT(idx >= MOTOR_CNT, "motor idx error");
    Motor_cfg_extern_t* ext = &motors_extern_cfgs[idx];
    motor_cfg_struct* cfg = &motors_cfg[idx];
    cfg->zero_cordinate = ext->zero * ext->step_ratio * cfg->subdivision;
    cfg->max_cordinate = ext->max * ext->step_ratio * cfg->subdivision;
    cfg->lost_endure_continue = ext->lost_continue * ext->step_ratio * cfg->subdivision;
    cfg->lost_endure_stop = ext->lost_stop * ext->step_ratio * cfg->subdivision;
}

int motor_step_calc(u8 idx, float value) {
    ASSERT(idx >= MOTOR_CNT, "motor idx error");
    Motor_cfg_extern_t* ext = &motors_extern_cfgs[idx];
    motor_cfg_struct* cfg = &motors_cfg[idx];
    return value * ext->step_ratio * cfg->subdivision;
}

f32 motor_distance_calc(u8 idx, u32 value) {
    ASSERT(idx >= MOTOR_CNT, "motor idx error");
    Motor_cfg_extern_t* ext = &motors_extern_cfgs[idx];
    motor_cfg_struct* cfg = &motors_cfg[idx];
    return 1.0 * value / ext->step_ratio / cfg->subdivision;
}


void motor_cfg_change_by_extern(int idx){
    motor_cfg_struct* cfg = &motors_cfg[idx];
    Motor_cfg_extern_t *extend = &motors_extern_cfgs[idx];
    cfg->max_cordinate = motor_distance_calc(idx, extend->max);
    cfg->zero_cordinate = motor_distance_calc(idx, extend->zero);
    cfg->lost_endure_continue = motor_distance_calc(idx, extend->lost_continue);
    cfg->lost_endure_stop = motor_distance_calc(idx, extend->lost_stop);   
}

static void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data) {
    Motor_queue_ele_t element;
    element.cmd_code = itf_code;
    element.packet_head = *packet_head;
    element.p_cmd = data;
    SeqQueue_Enter(&queue[idx], &element);
}

static void free_seq_args(int cnt, motor_seq_move_args_t* seqArgs)
{
	for(int i = 0; i < cnt; i++)
	{
		free(seqArgs[i].args);
	}
	free(seqArgs);
}

void motor_cmd_run(u32 itf_code, bus_packet* packet){
    switch(itf_code){
        case 0x00010000:{
            Itf_00010000_t* cmd = Itf_00010000_new();
            cmd->packet_parse(cmd, packet);
            u8 idx = cmd->content.idx;
            //修改当前电流
            if(cmd->content.current_level.data_u.c){
                motors_cfg[idx].currentLevel = cmd->content.current_level.data_u.d;
            }
            if(cmd->content.hold_level.data_u.c){
                motors_cfg[idx].holdLevel = cmd->content.hold_level.data_u.d;
            }
            if(cmd->content.subdivision.data_u.c){
                motors_cfg[idx].subdivision = cmd->content.subdivision.data_u.d;
                //进行距离到步数的换算
                motor_cordinate_update(idx);
            }
            break;
        }
        case 0x00010010:{
            Itf_00010010_t* cmd = Itf_00010010_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.id, itf_code, packet, cmd);
            break;
        }
        case 0x00010020:{
            Itf_00010020_t* cmd = Itf_00010020_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.idx, itf_code, packet, cmd);
            break;
        }
        case 0x00010030:{
            Itf_00010030_t* cmd = Itf_00010030_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.idx_rel.data_u.idx, itf_code, packet, cmd);
            break;
        }
        case 0x00010031:{
            Itf_00010031_t* cmd = Itf_00010031_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.idx_rel.data_u.idx, itf_code, packet, cmd);
            break;
        }
        case 0x00010040:{
            Itf_00010040_t* cmd = Itf_00010040_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.idx, itf_code, packet, cmd);
            break;
        }
        case 0x00010050:{
            Itf_00010050_t* cmd = Itf_00010050_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.idx_dir.data_u.idx, itf_code, packet, cmd);
            break;
        }
        case 0x00010060:{
            Itf_00010060_t* cmd = Itf_00010060_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.idx, itf_code, packet, cmd);
            break;
        }
        case 0x00010070:{
            Itf_00010070_t* cmd = Itf_00010070_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.idx_rel.data_u.idx, itf_code, packet, cmd);
            break;
        }
        case 0x00010080:{
            Itf_00010080_t* cmd = Itf_00010080_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content_array[0].idx_rel.data_u.idx, itf_code, packet, cmd);
            break;
        }
        case 0x00010082:{
            Itf_00010082_t* cmd = Itf_00010082_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content_array[0].args[0].idx_rel.data_u.idx, itf_code, packet, cmd);
            break;
        }
        default:ASSERT(1,"motor cmd error");break;
    }
    
}

static int thread_idx = 0;
void  Motor_Thread(void *arg)
{
    int td_idx = thread_idx++;
	while(1)
    {			  
		SeqQueue_Waite(&queue[td_idx])	;
		Motor_queue_ele_t ele;					
		BUS_ASSERT(!SeqQueue_Delete(&queue[td_idx],&ele),"Motor_Thread Seq Delete");
        BUS_ASSERT(SeqQueue_GetSize(&queue[td_idx])!=0,"Motor_Thread Seq Size");			
        switch(ele.cmd_code)
        {
            case 0x00010020:
            {
                Itf_00010020_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx;
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
				motors[idx].speed = motor_step_calc(idx, p_cmd->content.speed);
                motors[idx].acc = motor_step_calc(idx, p_cmd->content.acce);
				u8 error_code=motors[idx].mTMC5130MotorReset(&motors[idx]);
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);
				break;
            }            
            case 0x00010030:
            {
				printf("CMD_MotorMove\r\n");
                Itf_00010030_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx_rel.data_u.idx;
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
                motors[idx].postion = motor_step_calc(idx, p_cmd->content.distance);
				motors[idx].speed = motor_step_calc(idx, p_cmd->content.speed);
                motors[idx].acc = motor_step_calc(idx, p_cmd->content.acce);
                motors[idx].isBackZero=0;
                motors[idx].isRelative=p_cmd->content.idx_rel.data_u.is_rel;
				u8 error_code = motors[idx].mTMC5130MotorMove(&motors[idx]);
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);		
                break;								
            }
            case 0x00010031:
            {
				printf("CMD_MotorMoveNotify\r\n");
                Itf_00010031_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx_rel.data_u.idx;
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
                motors[idx].postion = motor_step_calc(idx, p_cmd->content.distance);
				motors[idx].speed = motor_step_calc(idx, p_cmd->content.speed);
                motors[idx].acc = motor_step_calc(idx, p_cmd->content.acce);
                motors[idx].isBackZero=0;
                motors[idx].isRelative=p_cmd->content.idx_rel.data_u.is_rel;
				u8 error_code = motors[idx].mTMC5130MotorMove(&motors[idx]);
                motor_move_done_notify(p_cmd->content.idx_rel.data_u.event_id);
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);		
                break;								
            }
            case 0x00010040 ://己测试
            {
				printf("CMD_MotorMove\r\n");
                Itf_00010040_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx;
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
				motors[idx].speed = motor_step_calc(idx, p_cmd->content.speed);
                motors[idx].acc = motor_step_calc(idx, p_cmd->content.acce);
                motors[idx].isBackZero=1;
                motors[idx].isRelative=0;
				u8 error_code = motors[idx].mTMC5130MotorMove(&motors[idx]);
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);		
                break;								
            }
			case 0x00010050 :
            {
                printf("CMD_MotorRotateStart\r\n");
                Itf_00010050_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx_dir.data_u.idx;
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
				motors[idx].speed = motor_step_calc(idx, p_cmd->content.speed);
                motors[idx].acc = motor_step_calc(idx, p_cmd->content.acce);
                motors[idx].dir=p_cmd->content.idx_dir.data_u.is_forward;
				u8 error_code = motors[idx].mTMC5130MotorRotate(&motors[idx]);
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);	
                break;
            }            
			case 0x00010060 :
            {
                printf("CMD_MotorRotateStop\r\n");
                Itf_00010060_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx;
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
               
				u8 error_code = motors[idx].mTMC5130MotorStop(&motors[idx]);		
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);	
                break;
            }	
            case 0x00010070 : 
            {
				printf("0x00010070 \r\n");
                Itf_00010070_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx_rel.data_u.idx;
				motors[idx].id=idx;
				motors[idx].acc=motor_step_calc(idx, p_cmd->content.acce);
				motors[idx].speed=motor_step_calc(idx, p_cmd->content.speed);
				motors[idx].postion=motor_step_calc(idx, p_cmd->content.distance);
				motors[idx].isRelative=p_cmd->content.idx_rel.data_u.is_rel;
                int coordinate = 0;
                u8 error_code=motors[idx].mTMC5130MotorMoveStopEvent(&motors[idx], &coordinate);
                if(error_code != 0)
                {	
                    coordinate = 0;
                }
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code, motor_distance_calc(idx, coordinate));
                break;
            } 
			case 0x00010080:
            {
				printf("CMD_MotorMultiMove\r\n");				
                Itf_00010080_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content_array[0].idx_rel.data_u.idx;
                motors[idx].id = idx;
				ASSERT(idx != td_idx || p_cmd->content_len == 0, "m1_idx != td_idx || p_cmd->content_len == 0");
                motor_multi_move_args_t * margs = malloc(sizeof(motor_multi_move_args_t)*p_cmd->content_len);
                if(margs == NULL)
                {
                    ASSERT(idx != td_idx || p_cmd->content_len == 0, "CMD_MotorMultiMove margs == NULL");
                }
                else
                {
                    memset(margs,0,sizeof(motor_multi_move_args_t)*p_cmd->content_len);                  
                    for(char i=0;i<p_cmd->content_len;i++)
                    {
                        u8 idx = p_cmd->content_array[i].idx_rel.data_u.idx;
                        ASSERT(idx >= MOTOR_CNT, "idx >= MOTOR_CNT");
                        margs[i].motor=&motors[idx];
                        margs[i].m1_idx=idx;
                        margs[i].m2_acce=motor_step_calc(idx, p_cmd->content_array[i].acce);
                        margs[i].m3_speed=motor_step_calc(idx, p_cmd->content_array[i].speed);
                        margs[i].m4_distance=motor_step_calc(idx, p_cmd->content_array[i].distance);;
                        margs[i].m5_isRelative=p_cmd->content_array[i].idx_rel.data_u.is_rel;
                        margs[i].m6_delayms=0;					
                        margs[i].motor->id=idx;	
                        margs[i].motor->wait_type=0;
                        margs[i].motor->evnt_group=0;
                        margs[i].motor->isBackZero=0;
                    }
                    u8 error_code = margs[0].motor->mTMC5130Motor_MultiMove(p_cmd->content_len, margs, 0);
					free(margs);                    
                    p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);
                }
                break;
            }  
            case 0x00010082:
            {
				printf("0x00010082\r\n");		
                Itf_00010082_t *p_cmd = ele.p_cmd;
                motor_seq_move_args_t* seqArgs = malloc(sizeof(motor_seq_move_args_t)*p_cmd->content_len);
                for(int i = 0; i < p_cmd->content_len; i++)
                {
                    motor_seq_move_args_t* seq = &seqArgs[i];
                    seq->args_len = p_cmd->content_array[i].arg_len;
                    seq->args = (motor_multi_move_args_t*)malloc(sizeof(motor_multi_move_args_t)*seq->args_len);
                    seq->m2_delayms = p_cmd->content_array[i].delay_ms;
                    for(int j = 0; j < seq->args_len;j++)
                    {
                        u8 idx = p_cmd->content_array[i].args[j].idx_rel.data_u.idx;
                        motor_multi_move_args_t* multi = &seq->args[j];
                        multi->motor=&motors[idx];
                        multi->m1_idx=idx;
                        multi->m2_acce=motor_step_calc(idx, p_cmd->content_array[i].args[j].acce);
                        multi->m3_speed=motor_step_calc(idx, p_cmd->content_array[i].args[j].speed);
                        multi->m4_distance=motor_step_calc(idx, p_cmd->content_array[i].args[j].distance);
                        multi->m5_isRelative=p_cmd->content_array[i].args[j].idx_rel.data_u.is_rel;
                        multi->motor->id=idx;
                    }
                }
                u8 error_code = seqArgs[0].args[0].motor->mTMC5130Motor_SeqMove(p_cmd->content_len, seqArgs);
                free_seq_args(p_cmd->content_len, seqArgs);
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);
                break;
            } 
        }            
    }
}

