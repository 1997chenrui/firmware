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
#include "bus.h"
#include "itf_01000007.h"
#include "itf_01000008.h"
#include "itf_01000009.h"
#include "runze_sv04.h"
#include "stm32f4xx.h"

#define VALVE_CHANNEL_CNT 10

extern UART_HandleTypeDef huart3;
Runze_t *runze;
u8 uart3_rx_buff[2];
static SeqQueue_Struct queue;
static bus_os_sem sem;

static void rv_ctrl_thread(void *data);
static void uart3_bytes_send(u8 *data, u8 len);
static bool queue_pend(void);
static bool queue_post(void);
static void queue_ele_enter(u32 itf_code, bus_packet *packet_head, void *data);

void rv_thread_init(void (*thread_create)(void (*thread)(void *data))) {
    runze = runze_new(VALVE_CHANNEL_CNT, uart3_bytes_send);
    SeqQueue_Init(&queue, sizeof(Rotatevalve_queue_ele_t), queue_pend,
                  queue_post);
    bus_os_create_sem(&sem, 0);
    thread_create(rv_ctrl_thread);
    HAL_UART_Receive_DMA(&huart3, uart3_rx_buff, 2);
}

void rv_cmd_run(u32 itf_code, bus_packet *packet) {
    switch (itf_code) {
    case 0x01000007: {
        Itf_01000007_t *cmd = Itf_01000007_new();
        cmd->packet_parse(cmd, packet);
        queue_ele_enter(itf_code, packet, cmd);
        break;
    }
    case 0x01000008: {
        Itf_01000008_t *cmd = Itf_01000008_new();
        cmd->packet_parse(cmd, packet);
        queue_ele_enter(itf_code, packet, cmd);
        break;
    }
    case 0x01000009: {
        Itf_01000009_t *cmd = Itf_01000009_new();
        cmd->packet_parse(cmd, packet);
        queue_ele_enter(itf_code, packet, cmd);
        break;
    }
    default:
        ASSERT("1", "rotate valve cmd no support");
        break;
    }
}

static void rv_ctrl_thread(void *data) {
    printf("## rv thread start ##\r\n");
    while (1) {
        SeqQueue_Waite(&queue);
        Rotatevalve_queue_ele_t ele;
        ASSERT(!SeqQueue_Delete(&queue, &ele), "rv_ctrl_thread Seq Delete");
        ASSERT(SeqQueue_GetSize(&queue) != 0, "rv_ctrl_thread Seq Size");
        //指令分发
        switch (ele.cmd_code) {
        case 0x01000007: {
            Itf_01000007_t *p_cmd = ele.p_cmd;
            runze->run_reset(runze);
            p_cmd->rtn_and_free(p_cmd, &ele.packet_head, 0);
            break;
        }
        case 0x01000008: {
            Itf_01000008_t *p_cmd = ele.p_cmd;
            runze->run_auto_channel(runze, p_cmd->content.channel);
            p_cmd->rtn_and_free(p_cmd, &ele.packet_head, 0);
            break;
        }
        case 0x01000009: {
            Itf_01000009_t *p_cmd = ele.p_cmd;
            runze->run_auto_channel_between(runze);
            p_cmd->rtn_and_free(p_cmd, &ele.packet_head, 0);
            break;
        }
        default:
            ASSERT("1", "rotate valve thread cmd no support");
            break;
        }
    }
}

static void uart3_bytes_send(u8 *data, u8 len) {
    HAL_UART_Transmit(&huart3, data, len, 0xff);
}

static bool queue_pend(void) {
    bus_os_pend_sem(&sem, 0);
    return true;
}

static bool queue_post(void) {
    bus_os_post_sem(&sem);
    return true;
}

static void queue_ele_enter(u32 itf_code, bus_packet *packet_head, void *data) {
    Rotatevalve_queue_ele_t element;
    element.cmd_code = itf_code;
    element.packet_head = *packet_head;
    element.p_cmd = data;
    SeqQueue_Enter(&queue, &element);
}
