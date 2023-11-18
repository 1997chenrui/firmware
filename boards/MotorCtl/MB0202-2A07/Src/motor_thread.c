#include "motor_thread.h"
#include "Protocols.h"
#include "cmd.h"
#include "mTMC5130Device.h"
#include "bus.h"
#include "motor_cfg.h"
#include "project_cfg.h"
#include "itf_00010000.h"
#include "itf_00010010.h"
#include "itf_00010020.h"
#include "itf_00010030.h"
#include "itf_00010031.h"
#include "itf_00010040.h"
#include "itf_00010050.h"
#include "itf_00010060.h"
#include "itf_00010062.h"
#include "itf_00010070.h"
#include "itf_00010071.h"
#include "itf_00010080.h"
#include "itf_00010082.h"
#include "itf_00010083.h"
#include "itf_01000b04.h"

extern MotorThread MotorThread_OS[];
//队列成员应该是命令码和send对象	
extern SeqQueue_Struct   queue[];

///<电机参数配置表
extern motor_cfg_struct motors_cfg[];

extern DRIVER_REG motors_reg[];	

extern MOTOR motors[];

extern Motor_cfg_extern_t motors_extern_cfgs[];

__weak u8 wait_motor_seq_move_start(u8 signal_id, u16 timeout_ms){
    return 1;
}

__weak u8 motor_cmd_extend(u32 itf_code, bus_packet* packet){
    return 1;
}

__weak u8 motor_thread_exe_extend(Motor_queue_ele_t * ele){
    return 1;
}


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

//void MotorThread_EnQueue(int idx,Motor_Queue_Ele_t *data)
//{
//    SeqQueue_Enter(&queue[idx],data);
//}

void motor_cordinate_update(u8 idx) {//电机坐标更新
    ASSERT(idx >= MOTOR_CNT, "motor idx error");
    Motor_cfg_extern_t* ext = &motors_extern_cfgs[idx];
    motor_cfg_struct* cfg = &motors_cfg[idx];
    cfg->zero_cordinate = ext->zero * ext->step_ratio * cfg->subdivision + 0.5f;
    cfg->max_cordinate = ext->max * ext->step_ratio * cfg->subdivision + 0.5f;
    cfg->lost_endure_continue = ext->lost_continue * ext->step_ratio * cfg->subdivision + 0.5f;
    cfg->lost_endure_stop = ext->lost_stop * ext->step_ratio * cfg->subdivision + 0.5f;
}

int motor_step_calc(u8 idx, float value) {
    ASSERT(idx >= MOTOR_CNT, "motor idx error");
    Motor_cfg_extern_t* ext = &motors_extern_cfgs[idx];
    motor_cfg_struct* cfg = &motors_cfg[idx];
    return (value * ext->step_ratio * cfg->subdivision + 0.5f);
}

f32 motor_distance_calc(u8 idx, int value) {
    ASSERT(idx >= MOTOR_CNT, "motor idx error");
    Motor_cfg_extern_t* ext = &motors_extern_cfgs[idx];
    motor_cfg_struct* cfg = &motors_cfg[idx];
    f32 f_value = 1.0 * value / ext->step_ratio / cfg->subdivision;
//    printf("idx = %d, value = %d, f_value = %6f", idx, value, f_value);
    return f_value;
}

void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data) {
    Motor_queue_ele_t element;
    element.cmd_code = itf_code;
    element.packet_head = *packet_head;
    element.p_cmd = data;
    SeqQueue_Enter(&queue[idx], &element);
}

void free_seq_args(int cnt, motor_seq_move_args_t* seqArgs)
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
				//其实就是通过修改 motor_cfg文件中 对应电机的参数来设置
            }
            if(cmd->content.hold_level.data_u.c){
                motors_cfg[idx].holdLevel = cmd->content.hold_level.data_u.d;
            }
            if(cmd->content.subdivision.data_u.c){
                motors_cfg[idx].subdivision = cmd->content.subdivision.data_u.d;
                //进行距离到步数的换算
                motor_cordinate_update(idx);
            }
            cmd->rtn_and_free(cmd, packet, 0);
            break;
        }
        case 0x00010010:{//获取电机状态
            Itf_00010010_t* cmd = Itf_00010010_new();
            cmd->packet_parse(cmd, packet);
            MOTOR* motor = &motors[cmd->content.id];
            motor_cfg_struct* cfg = &motors_cfg[cmd->content.id];
            Itf_00010010_rtn_t rtn;
            rtn.is_zero_trigger = motor->read_io(PIN_ID_SET_SENSOR,motor->zeor_isr_id,1) == motor->mot_cfg->zero_trig_vol;
            rtn.current_cordinate = motor_distance_calc(cmd->content.id, mTMC5130DeviceGetPostion(motor,1));
            rtn.current_level = cfg->currentLevel;
            rtn.hold_level = cfg->holdLevel;
            rtn.lost_step = 0;
            rtn.subdivision = cfg->subdivision;
            cmd->rtn_and_free(cmd, packet, &rtn);
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
        case 0x00010062:{
            Itf_00010062_t* cmd = Itf_00010062_new();
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
        case 0x00010071:{
            Itf_00010071_t* cmd = Itf_00010071_new();
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
        case 0x00010083:{
            Itf_00010083_t* cmd = Itf_00010083_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.seq_array[0].args[0].idx_rel.data_u.idx, itf_code, packet, cmd);
            break;
        }
		case 0x01000b04:{
			Itf_01000b04_t* cmd = Itf_01000b04_new();
            cmd->packet_parse(cmd, packet);
			if(cmd->content.idx)
			{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
			}
			cmd->rtn_and_free(cmd,packet,0);
			break;
			
		}
        default:{
			//若此符号未被覆盖 说明指令未找到
            //若此符号被覆盖，说明存在指令扩展，若指令扩展返回1，说明扩展指令中仍未找到该指令
            if(motor_cmd_extend(itf_code, packet)){  
				ASSERT(1,"motor cmd error");
            }
            break;
		}
    }
    
}

static int thread_idx = 0;
void  Motor_Thread(void *arg)
{
    int td_idx = thread_idx++;
	printf("motor thread %d start\r\n", td_idx);
	while(1)
    {			  
		SeqQueue_Waite(&queue[td_idx])	;//等待一个新的 电机操作消息队列
		Motor_queue_ele_t ele;					
		BUS_ASSERT(!SeqQueue_Delete(&queue[td_idx],&ele),"Motor_Thread Seq Delete");
        BUS_ASSERT(SeqQueue_GetSize(&queue[td_idx])!=0,"Motor_Thread Seq Size");			
        switch(ele.cmd_code)
        {
            case 0x00010020://复位
            {
                Itf_00010020_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx;
//				if(idx == 0)
//				{
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);//刹车电机解锁
////					printf("LK-succ\r\n");
//				}
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
				motors[idx].speed = motor_step_calc(idx, p_cmd->content.speed);//计算出电机需要工作的步数
                motors[idx].acc = motor_step_calc(idx, p_cmd->content.acce);
				
				u8 error_code=motors[idx].mTMC5130MotorReset(&motors[idx]);
//				if(idx == 0)
//				{
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);//刹车电机上锁
////					printf("ulk\r\n");
//				}
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);
				break;
            }            
            case 0x00010030:
            {
				printf("CMD_MotorMove\r\n");
                Itf_00010030_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx_rel.data_u.idx;
//				if(idx == 0)
//				{
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);//刹车电机解锁
////					printf("lk\r\n");
//				}
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
                motors[idx].postion = motor_step_calc(idx, p_cmd->content.distance);
				motors[idx].speed = motor_step_calc(idx, p_cmd->content.speed);
                motors[idx].acc = motor_step_calc(idx, p_cmd->content.acce);
                motors[idx].isBackZero=0;
				printf("p_cmd->content.distance = %f p_cmd->content.speed = %f p_cmd->content.acce = %f\r\n",p_cmd->content.distance,p_cmd->content.speed,p_cmd->content.acce);
                printf("p_cmd->content.idx_rel.data_u.is_rel = %d\r\n",p_cmd->content.idx_rel.data_u.is_rel);
				motors[idx].isRelative=p_cmd->content.idx_rel.data_u.is_rel;
				
				u8 error_code = motors[idx].mTMC5130MotorMove(&motors[idx]);
//				if(idx == 0)
//				{
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);//刹车电机上锁
////					printf("ulk\r\n");
//				}
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
//				if(idx == 0)
//				{
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);//刹车电机解锁
////					printf("lk\r\n");
//				}
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
				motors[idx].speed = motor_step_calc(idx, p_cmd->content.speed);
                motors[idx].acc = motor_step_calc(idx, p_cmd->content.acce);
                motors[idx].isBackZero=1;
                motors[idx].isRelative=0;
				u8 error_code = motors[idx].mTMC5130MotorMove(&motors[idx]);
//				if(idx == 0)
//				{
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);//刹车电机上锁
////					printf("ulk\r\n");
//				}
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
			case 0x00010062 :
            {
                printf("CMD_DISABLE\r\n");
                Itf_00010062_t *p_cmd = ele.p_cmd;
                u8 idx = p_cmd->content.idx;
                motors[idx].id = idx;
				ASSERT(idx != td_idx, "m1_idx != td_idx");
				HAL_GPIO_WritePin(motors_extern_cfgs[idx].en_port,motors_extern_cfgs[idx].en_pin,(GPIO_PinState)p_cmd->content.state);
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, 0);	
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
            case 0x00010071 : 
            {
				printf("0x00010071 \r\n");
                Itf_00010071_t *p_cmd = ele.p_cmd;
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
                }else {
                    motors[idx].postion=motor_step_calc(idx, p_cmd->content.distance_when_trigger);
                    motors[idx].isRelative=1;
                    motors[idx].isBackZero = 0;
                    motors[idx].mTMC5130MotorMove(&motors[idx]);
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
            case 0x00010083:
            {
				printf("0x00010083\r\n");
                Itf_00010083_t *p_cmd = ele.p_cmd;
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
                if(wait_motor_seq_move_start(p_cmd->content.signal_id, p_cmd->content.timeout_ms) == 0){
                    error_code = seqArgs[0].args[0].motor->mTMC5130Motor_SeqMove(p_cmd->content.seq_len, seqArgs);
                }else{
                    error_code = MOTOR_STATUS_ERROR_TIME_OUT;
                }
                free_seq_args(p_cmd->content.seq_len, seqArgs);
                p_cmd->rtn_and_free(p_cmd, &ele.packet_head, error_code);
                break;
            } 
			default:{
				//若此符号未被覆盖 说明指令未找到
				//若此符号被覆盖，说明存在指令扩展，若指令扩展返回1，说明扩展指令中仍未找到该指令
				if(motor_thread_exe_extend(&ele)){  
					ASSERT(1,"motor cmd error");
				}
				break;
			}
        }            
    }
}

/** 测试函数 **/
void motor_move_test(u8 idx, f32 distance, u8 is_rel){
	
	
		printf("Now move_test %d motor\r\n",idx);
	
    motors[idx].id = idx;
    motors[idx].postion = motor_step_calc(idx, distance);
    motors[idx].speed = motor_step_calc(idx, 100);
    motors[idx].acc = motor_step_calc(idx, 50);
    motors[idx].isBackZero=0;
    motors[idx].isRelative=is_rel;
    motors[idx].mTMC5130MotorMove(&motors[idx]);
}

/** 测试函数 **/
void motor_rotate_test(u8 idx, u16 time_ms, u8 dir){
	
		printf("Now rotate_test %d motor\r\n",idx);
	
    motors[idx].id = idx;
    motors[idx].speed = motor_step_calc(idx, 100);
    motors[idx].acc = motor_step_calc(idx, 50);
    motors[idx].dir = dir;
    motors[idx].mTMC5130MotorRotate(&motors[idx]);
	OSTimeDly(time_ms);
	motors[idx].mTMC5130MotorStop(&motors[idx]);
}

void motor_reset_test(u8 idx){
	
	printf("Now rest_test %d motor\r\n",idx);
    motors[idx].id = idx;
    motors[idx].speed = motor_step_calc(idx, 100);
    motors[idx].acc = motor_step_calc(idx, 50);
    motors[idx].mTMC5130MotorReset(&motors[idx]);
}
