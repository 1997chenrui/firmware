#include "bus.h"
#include "motor_thread.h"
#include "mTMC5130Device.h"
#include "itf_01000c30.h"
#include "itf_00040000.h"
#include "ad_signal.h"

typedef struct{
	u16 signal_array[1024];
	u16 write_idx;
}Signal_buffer_t;

Signal_buffer_t buffer;

extern MOTOR motors[];
extern void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
extern int motor_step_calc(u8 idx, float value);

static void signal_read(u8 idx, u16 time_ms, u16 time_span_ms);
static void buffer_write(Signal_buffer_t* self, u32 value);
static u16* buffer_ptr(Signal_buffer_t* self);
static u16 buffer_get_len(Signal_buffer_t* self);
static void buffer_clear(Signal_buffer_t* self);

u8 motor_cmd_extend(u32 itf_code, bus_packet* packet){
	switch(itf_code){
	    case 0x01000c30:{
            Itf_01000c30_t* cmd = Itf_01000c30_new();
            cmd->packet_parse(cmd, packet);
            queue_ele_enter(cmd->content.id, itf_code, packet, cmd);
            break;
		}
		default:return 1;
	}
	return 0;
}

u8 motor_thread_exe_extend(Motor_queue_ele_t * ele){
    switch(ele->cmd_code){
        case 0x01000c30:{
			printf("CMD_MotorRotateStart\r\n");
			Itf_01000c30_t *p_cmd = ele->p_cmd;
			u8 idx = p_cmd->content.id;
			idx+=1;
			motors[idx].id = idx;
			motors[idx].speed = motor_step_calc(idx, p_cmd->content.speed);
			motors[idx].acc = motor_step_calc(idx, 100);
			motors[idx].dir = 1;
			u8 error_code = motors[idx].mTMC5130MotorRotate(&motors[idx]);
			signal_read(p_cmd->content.id, p_cmd->content.rotate_ms, p_cmd->content.time_spam_ms);
			motors[idx].mTMC5130MotorStop(&motors[idx]);
			p_cmd->rtn_and_free(p_cmd, &ele->packet_head, buffer_ptr(&buffer), buffer_get_len(&buffer));
			break;
        }
        default:return 1;
    }
    return 0;
}

u8 bus_ita_func_callback_extern(u32 func_code, bus_packet * packet){
	switch(func_code){
		case 0x01000c30: {
			motor_cmd_run(func_code, packet);
			break;
		}
		default:return 1;
	}
    return 0;
}

static void signal_read(u8 idx, u16 time_ms, u16 time_span_ms) {
	u16 cnt = time_ms / time_span_ms;
	buffer_clear(&buffer);
	//电机序列号与ad检测序列号一一对应
	Signal_read_t* reader = signal_reader_as_ref();
	// 发送给上位机的总数据长度
	buffer_write(&buffer, cnt * 2);
	for(int i = 0; i < cnt; i++){
		buffer_write(&buffer, reader->hd_signal_read_two(idx));
		OSTimeDly(time_span_ms);
	}
}

static void buffer_write(Signal_buffer_t* self, u32 value){
	ASSERT(self->write_idx >= 512-1, "Signal Buffer full");
	WriteU32((u8*)(&self->signal_array[self->write_idx]), value);
	self->write_idx+=2;
}

static u16* buffer_ptr(Signal_buffer_t* self){
	return self->signal_array;
}

static u16 buffer_get_len(Signal_buffer_t* self){
	return self->write_idx;
}

static void buffer_clear(Signal_buffer_t* self){
	self->write_idx = 0;
}


