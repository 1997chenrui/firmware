#ifndef __VOLTAGE_H__
#define __VOLTAGE_H__
#include "macro.h"
#include "bus.h"
#include "data_struct.h"

#define VOLTAGE_THREAD_STACK_SIZE         256

#define  VOLTAGE_THRAD_CREATE(prio, idx)        \
bus_create_thread(&voltage_thread_os[idx].tcb,voltage_thread,         \
				voltage_thread_os[idx].stack, VOLTAGE_THREAD_STACK_SIZE, prio, NULL);\
bus_os_create_sem(&voltage_thread_os[idx].sem,0);            \
SeqQueue_Init(&voltage_queue[idx],sizeof(Voltage_queue_ele_t),&queue##idx##_pend,&queue##idx##_post)

#define VOLTAGE_CREATE_Q_BLOCK(idx)                                            \
static bool queue##idx##_pend()            \
{                                        \
	bus_os_pend_sem(&voltage_thread_os[idx].sem,0);    \
	return true;\
}                                        \
static bool queue##idx##_post()         \
{                                        \
	bus_os_post_sem(&voltage_thread_os[idx].sem);    \
	return true;                        \
}

typedef struct
{
	bus_os_thread               tcb;
	bus_os_sem                  sem;
	u32                         stack[VOLTAGE_THREAD_STACK_SIZE]; 
}Voltage_thread_t;


typedef struct{
    u32         cmd_code;
    bus_packet  packet_head;
    void * p_cmd;
}Voltage_queue_ele_t;

typedef struct _Voltage_t {
    f32 max_voltage;
    f32 voltage_range[2];
	f32 current_voltage;
	u16 last_time;
    void (*hd_voltage_write)(f32 voltage);
	void (*hd_voltage_write_time)(f32, u16);
	void (*voltage_write_buffer_time)(struct _Voltage_t *);
    void (*voltage_write)(struct _Voltage_t *self, f32 voltage);
    void (*voltage_write_percent)(struct _Voltage_t *self, u8 voltage);
	void (*voltage_write_persent_time)(struct _Voltage_t *self, u8 voltage, u16 time);
    void (*voltage_write_u8)(struct _Voltage_t *self, u8 voltage);
} Voltage_t;

void voltage_init(void);
Voltage_t* voltage_get_as_ref(int idx);
void voltage_cmd_run(u32 func_code, bus_packet* packet);
void voltage_thread(void *arg);

#endif
