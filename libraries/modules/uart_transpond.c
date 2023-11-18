#include "uart_transpond.h"
#include <stdlib.h>

static UART_SEND_BYTES pfunc_hd_send;
static void pack_rev_check(Uart_transpond_t *self);
static void byte_rev(Uart_transpond_t *self, u8 data);
static void data_send(Uart_transpond_t *self, u8 *data, u32 data_len);

Uart_transpond_t *uart_transpond_new(u8 id, UART_SEND_BYTES send) {
    if (pfunc_hd_send == NULL) {
        pfunc_hd_send = send;
    }
    Uart_transpond_t *transpond =
        (Uart_transpond_t *)malloc(sizeof(Uart_transpond_t));
    transpond->id = id;
    transpond->daemon_time = 0;
    memset(&transpond->fifo, 0, sizeof(Uart_fifo_t));
    transpond->byte_rev = byte_rev;
    transpond->data_send = data_send;
    transpond->pack_rev_check = pack_rev_check;
    return transpond;
}

/**
 * @brief 写fifo
 *
 * @param self
 * @param buff
 * @param length
 * @return u8
 */
static u8 fifo_write(Uart_fifo_t *self, u8 *buff, u16 length) {
    if (UARTFIFO_CNT - self->size < length) {
        self->size = UARTFIFO_CNT;
        self->full = 1;
        printf("[error]fifo full\r\n");
        return 1;
    } else {
        if (self->wr_p + length > UARTFIFO_CNT - 1) {
            memmove(self->buff + self->wr_p, buff,
                    UARTFIFO_CNT - 1 - self->wr_p);
            memmove(self->buff, buff + UARTFIFO_CNT - 1 - self->wr_p,
                    self->wr_p + length - (UARTFIFO_CNT - 1));
            self->wr_p = self->wr_p + length - (UARTFIFO_CNT - 1);
        } else {
            memmove(self->buff + self->wr_p, buff, length);
            self->wr_p += length;
        }
        self->size += length;
        self->empty = 0;
        if (self->size == UARTFIFO_CNT) {
            self->full = 1;
        }
    }
    return 0;
}

/**
 * @brief 读fifo
 *
 * @param self
 * @param buff
 * @param length
 * @return u16
 */
static u16 fifo_read(Uart_fifo_t *self, u8 *buff, u16 length) {
    if (self->size < length) {
        self->size = 0;
        self->empty = 1;
        printf("           [error]fifo empty\r\n");
        return 1;
    } else {
        if (self->rd_p + length > UARTFIFO_CNT - 1) {
            memmove(buff, self->buff + self->rd_p,
                    UARTFIFO_CNT - 1 - self->rd_p);
            memmove(buff + UARTFIFO_CNT - 1 - self->rd_p, self->buff,
                    self->rd_p + length - (UARTFIFO_CNT - 1));
            self->rd_p = self->rd_p + length - (UARTFIFO_CNT - 1);
        } else {
            memmove(buff, self->buff + self->rd_p, length);
            self->rd_p += length;
        }
        self->size -= length;
        if (self->wr_p == self->rd_p) {
            self->empty = 1;
        }
    }
    return 0;
}

static void data_send(Uart_transpond_t *self, u8 *data, u32 data_len) {
    pfunc_hd_send(self->id, data, data_len);
}

/**
 * @brief 从中断接收数据
 *
 * @param self
 * @param data
 */
static void byte_rev(Uart_transpond_t *self, u8 data) {
    fifo_write(&self->fifo, &data, 1);
    self->daemon_time = 0;
}

/**
 * @brief 在线程中判定是否接收完成
 *
 * @param self
 */
static void pack_rev_check(Uart_transpond_t *self) {
    // sunly添加BOX项目需要结束符来区分此条数据结束
    if (self->daemon_time > DAEMON_TIME_CNT ||
        self->fifo.size > DAEMON_SEND_THRESHOLD) {
        if (self->fifo.size > DAEMON_SEND_THRESHOLD) {
            u8 *temp_buff = (u8 *)malloc(DAEMON_SEND_THRESHOLD);
            fifo_read(&self->fifo, temp_buff, DAEMON_SEND_THRESHOLD);

            pfunc_hd_send(self->id, temp_buff, DAEMON_SEND_THRESHOLD);
            free(temp_buff);
        }
        /******当超时，且fifo.size大于设定值时，转发数据并加上结束符*******/
//        else if (self->daemon_time > DAEMON_TIME_CNT &&
//                 self->fifo.size > DAEMON_SEND_THRESHOLD) {
//            u8 *temp_buff = (u8 *)malloc(DAEMON_SEND_THRESHOLD);
//            fifo_read(&self->fifo, temp_buff, DAEMON_SEND_THRESHOLD);
//			
//            pfunc_hd_send(self->id, temp_buff, DAEMON_SEND_THRESHOLD);
//            free(temp_buff);
//            u8 UartReceiveEndFLAG[5] = {0, 0, '\n', 0, 0};
//            fifo_write(&self->fifo, UartReceiveEndFLAG, 5);
//            u8 *temp1_buff = (u8 *)malloc(self->fifo.size);
//            u32 temp_size = self->fifo.size;
//            fifo_read(&self->fifo, temp1_buff, temp_size);
//            pfunc_hd_send(self->id, temp1_buff, temp_size);
//            free(temp1_buff);
//        }

        else if (self->daemon_time > DAEMON_TIME_CNT && self->fifo.size > 0) {
            u8 UartReceiveEndFLAG[5] = {0, 0, '\n', 0, 0};
            fifo_write(&self->fifo, UartReceiveEndFLAG, 5);
            u8 *temp_buff = (u8 *)malloc(self->fifo.size);
            u32 temp_size = self->fifo.size;
            fifo_read(&self->fifo, temp_buff, temp_size);
            pfunc_hd_send(self->id, temp_buff, temp_size);
            free(temp_buff);
        }
    } else {
        self->daemon_time += DAEMON_TIME_SPACE;
    }
}
