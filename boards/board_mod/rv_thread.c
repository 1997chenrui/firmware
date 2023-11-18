/**
 * @file rv_thread.c
 * @author wangz
 * @brief
 * 旋转阀控制线程，单线程内不允许缓存指令，在一个指令执行完成之前不允许下发其他指令。
 * @version 0.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "rv_thread.h"
#include "itf_000100a0.h"
#include "itf_000100a1.h"
#include "itf_000100a2.h"
#include "stm32f4xx.h"
#include "rv_cfg.h"

#define VALVE_CHANNEL_CNT 10
extern u8 reset_channel_set(Runze_t *self);
extern Rv_thread_t rv[];
extern void rv_hd_init(void);
static void rv_ctrl_thread(void *data);
static void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data);

void rv_thread_init(void) {
    rv_hd_init();
    for(int i = 0; i < RUNZE_CNT;i++){
        Rv_thread_t* self = &rv[i];
        u8* thread_parameter = (u8*)malloc(1);
        *thread_parameter = i;
        self->runze = runze_new(VALVE_CHANNEL_CNT, self->uart_bytes_send);
		//		reset_channel_set(self->runze);//设置默认复位通道10;（此时复位速度太慢）
        SeqQueue_Init(&self->queue, sizeof(Rotatevalve_queue_ele_t), self->queue_pend,
                      self->queue_post);
        bus_os_create_sem(&self->queue_sem, 0);
        bus_create_thread(&self->thread_tcb,rv_ctrl_thread,         
				self->thread_stack, RV_THREAD_STACK_SIZE, self->thread_prio, thread_parameter);
    }
}

void rv_cmd_run(u32 itf_code, bus_packet *packet) {
    switch (itf_code) {
    case 0x000100a0: {
        Itf_000100a0_t *cmd = Itf_000100a0_new();
        cmd->packet_parse(cmd, packet);
		printf("%d\r\n",cmd->content.id);
        queue_ele_enter(cmd->content.id, itf_code, packet, cmd);
        break;
    }
    case 0x000100a1: {
        Itf_000100a1_t *cmd = Itf_000100a1_new();
        cmd->packet_parse(cmd, packet);
		printf("%d\r\n",cmd->content.id);
        queue_ele_enter(cmd->content.id, itf_code, packet, cmd);
        break;
    }
    case 0x000100a2: {
        Itf_000100a2_t *cmd = Itf_000100a2_new();
        cmd->packet_parse(cmd, packet);
		printf("%d\r\n",cmd->content.id);
        queue_ele_enter(cmd->content.id, itf_code, packet, cmd);
        break;
    }
    default:
        ASSERT("1", "rotate valve cmd no support");
    }
}

static void rv_ctrl_thread(void *data) {
    u8 idx = ((u8*)data)[0];
    printf("## rv thread %d start ##\r\n", idx);
    free(data);
    Rv_thread_t* p_rv = &rv[idx];
    while (1) {
        SeqQueue_Waite(&p_rv->queue);
        Rotatevalve_queue_ele_t ele;
        ASSERT(!SeqQueue_Delete(&p_rv->queue, &ele), "rv_ctrl_thread Seq Delete");
        ASSERT(SeqQueue_GetSize(&p_rv->queue) != 0, "rv_ctrl_thread Seq Size");
        Runze_t *self =  p_rv->runze;
        //指令分发
        switch (ele.cmd_code) {
        case 0x000100a0: {
            Itf_000100a0_t *p_cmd = ele.p_cmd;
            self->run_reset(self);
            p_cmd->rtn_and_free(p_cmd, &ele.packet_head, 0);
            break;
        }
        case 0x000100a1: {
            Itf_000100a1_t *p_cmd = ele.p_cmd;
            self->run_auto_channel(self, p_cmd->content.channel);
            p_cmd->rtn_and_free(p_cmd, &ele.packet_head, 0);
            break;
        }
        case 0x000100a2: {
            Itf_000100a2_t *p_cmd = ele.p_cmd;
            self->run_auto_channel_between(self);
            p_cmd->rtn_and_free(p_cmd, &ele.packet_head, 0);
            break;
        }
        default:
            ASSERT("1", "rotate valve thread cmd no support");
        }
    }
}

static void queue_ele_enter(u8 idx, u32 itf_code, bus_packet *packet_head, void *data) {
    Rotatevalve_queue_ele_t element;
    element.cmd_code = itf_code;
    element.packet_head = *packet_head;
    element.p_cmd = data;
    SeqQueue_Enter(&rv[idx].queue, &element);
}
