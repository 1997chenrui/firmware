/**
 * @file light.c
 * @author wangz
 * @brief 通过电压调节光强
 * @version 0.1
 * @date 2022-05-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "voltage.h"
#include "itf_00070000.h"
#include "itf_00070001.h"
#include "itf_00070002.h"
#include "itf_00070003.h"
#include "itf_00070011.h"
#include "voltage_cfg.h"

#if VOLTAGE_THREAD_ENABLE == 1
extern SeqQueue_Struct   voltage_queue[];
static void voltage_queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);
#endif
extern Voltage_t vol_array[];

static void voltage_write(Voltage_t* self, f32 voltage);
static void voltage_write_buffer_time(Voltage_t* self);
static void voltage_write_persent(Voltage_t* self, u8 persent);
static void voltage_write_u8(Voltage_t* self, u8 code);
static void voltage_write_persent_time(Voltage_t* self, u8 persent, u16 time_ms);
static f32 limit(Voltage_t* self, f32 voltage);

void voltage_init(void) {
	voltage_hd_init();
	for(int i = 0; i < VOLTAGE_CHANNEL_CNT; i++){
		vol_array[i].voltage_write = voltage_write;
		vol_array[i].voltage_write_buffer_time = voltage_write_buffer_time;
		vol_array[i].voltage_write_percent = voltage_write_persent;
		vol_array[i].voltage_write_u8 = voltage_write_u8;
		vol_array[i].voltage_write_persent_time = voltage_write_persent_time;
	}
	
// test
#if 0
	voltage_write_persent_time(&vol_array[0], 40,2000);
#endif
}

Voltage_t* voltage_get_as_ref(int idx){
	ASSERT(idx >= VOLTAGE_CHANNEL_CNT, "voltage id error");
	return &vol_array[idx];
}


void voltage_cmd_run(u32 func_code, bus_packet* packet){
    switch(func_code){
        case 0x00070000:{
            Itf_00070000_t* cmd = itf_00070000_new();
            cmd->packet_parse(cmd, packet);
            ASSERT(cmd->content.idx >= VOLTAGE_CHANNEL_CNT, "voltage id error");
            voltage_write(&vol_array[cmd->content.idx], cmd->content.voltage);
            cmd->rtn_and_free(cmd, packet, 0);
			break;
        }
        case 0x00070001:{
            Itf_00070001_t* cmd = itf_00070001_new();
            cmd->packet_parse(cmd, packet);
            ASSERT(cmd->content.idx >= VOLTAGE_CHANNEL_CNT, "voltage id error");
            voltage_write_persent(&vol_array[cmd->content.idx], cmd->content.persent);
            cmd->rtn_and_free(cmd, packet, 0);
			break;
        }
        case 0x00070002:{
            Itf_00070002_t* cmd = itf_00070002_new();
            cmd->packet_parse(cmd, packet);
            ASSERT(cmd->content.idx >= VOLTAGE_CHANNEL_CNT, "voltage id error");
            voltage_write_u8(&vol_array[cmd->content.idx], cmd->content.code);
			printf("idx=%d power=%d\r\n",cmd->content.idx,cmd->content.code);
            cmd->rtn_and_free(cmd, packet, 0);
			break;
		}
        case 0x00070003:{
            Itf_00070003_t* cmd = itf_00070003_new();
            cmd->packet_parse(cmd, packet);
            ASSERT(cmd->content.idx >= VOLTAGE_CHANNEL_CNT, "voltage id error");
			voltage_write(&vol_array[cmd->content.idx],(f32)cmd->content.code);

			printf("idx=%d power=%d\r\n",cmd->content.idx,cmd->content.code);
            cmd->rtn_and_free(cmd, packet, 0);
			break;
			
        }
#if VOLTAGE_THREAD_ENABLE == 1
		case 0x00070011:{
			Itf_00070011_t* cmd = itf_00070011_new();
			cmd->packet_parse(cmd, packet);
			ASSERT(cmd->content.idx >= VOLTAGE_CHANNEL_CNT, "voltage id error");
			voltage_queue_ele_enter(cmd->content.idx, func_code, packet, cmd);
			break;
		}
#endif
        default:ASSERT(1, "voltage_cmd_rum error\r\n");
    }
}

/**Voltage_t成员函数**/
static void voltage_write_buffer_time(Voltage_t* self){
	self->hd_voltage_write_time(self->current_voltage, self->last_time);
}

static void voltage_write(Voltage_t* self, f32 voltage){
	voltage = limit(self, voltage);
	self->current_voltage = voltage;
    self->hd_voltage_write(voltage);
}

static void voltage_write_persent(Voltage_t* self, u8 persent){
	if(persent > 100) persent = 100;
	voltage_write(self, self->max_voltage * persent / 100);
}

static void voltage_write_u8(Voltage_t* self, u8 code){
	if(code > 255) code = 255;
	voltage_write(self, self->max_voltage * code / 255);
}




/**
 * @brief 电压限制
 * 
 * @return f32 限制后的电压
 */
static f32 limit(Voltage_t* self, f32 voltage){
    if(voltage < self->voltage_range[0]){
        return self->voltage_range[0];
    }else if(voltage > self->voltage_range[1]){
        return self->voltage_range[1];
    }else{
        return voltage;
    }
}

static void voltage_write_persent_time(Voltage_t* self, u8 persent, u16 time_ms){
	f32 voltage = limit(self, self->max_voltage * persent / 100);
	self->current_voltage = voltage;
	self->last_time = time_ms;
    self->hd_voltage_write_time(voltage, time_ms);
}
#if VOLTAGE_THREAD_ENABLE == 1
static void voltage_queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data) {
    Voltage_queue_ele_t element;
    element.cmd_code = itf_code;
    element.packet_head = *packet_head;
    element.p_cmd = data;
    SeqQueue_Enter(&voltage_queue[idx], &element);
}

static int voltage_idx = 0;
void voltage_thread(void *arg){
	int td_idx = voltage_idx++;
	while(1)
    {			  
		SeqQueue_Waite(&voltage_queue[td_idx])	;
		Voltage_queue_ele_t ele;					
		ASSERT(!SeqQueue_Delete(&voltage_queue[td_idx],&ele),"voltage_thread Seq Delete");
        ASSERT(SeqQueue_GetSize(&voltage_queue[td_idx])!=0,"voltage_thread Seq Size");			
        Voltage_t* self = NULL;
		switch(ele.cmd_code)
        {
            case 0x00070011: {
				Itf_00070011_t* cmd = ele.p_cmd;
				ASSERT(cmd->content.idx > VOLTAGE_CHANNEL_CNT - 1,"voltage_thread 0x00070011 idx error");	
				self = &vol_array[cmd->content.idx];
				voltage_write_persent_time(self, (cmd->content.persent), cmd->content.time_ms);
				cmd->rtn_and_free(cmd, &ele.packet_head, 0);
				break;
			}
			default: ASSERT(1,"voltage_thread func_code not find");
        }            
    }
}
#endif
