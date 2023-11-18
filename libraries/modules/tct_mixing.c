#include "tct_mixing.h"
#include "tool.h"
#include <stdio.h>

static TCTMixing_t mix;
static VOLTAGE_WRITE m_voltage_write;
static void (*m_power)(u8 on);
static void mix_thread(void* data);
static void add(f32 run_voltage, u16 run_ms);
static void parse(bus_packet* packet);


/** TCTMixing_t成员函数 **/
void tctmixing_init(f32 start_voltage, u16 start_ms, VOLTAGE_WRITE voltage_write, void (*power)(u8 on), void (*thread_create)(void (*thread)(void* data)), void (sem_create)(bus_os_sem* sem)){
    mix.start_voltage = start_voltage;
    mix.start_ms = start_ms;
    m_voltage_write = voltage_write;
    m_power = power;
    mix.add = add;
    mix.parse = parse;
	sem_create(&mix.sem);
	thread_create(mix_thread);
    
}

TCTMixing_t* tctmixing_get_as_ref(void){
    return &mix;
}

static void run(TCTMixing_t* self){
    printf("runvol = %f,runms = %d\r\n", self->run_voltage, self->run_ms);
    m_power(1);
    m_voltage_write(self->start_voltage);
    bus_os_sleep(self->start_ms);
    m_voltage_write(self->run_voltage);
    bus_os_sleep(self->run_ms);
    m_voltage_write(self->stop_voltage);
    m_power(0);
    if(self->is_report){
        u8 data[] = {0};
        bus_ita_return(self->packet.adapterId,self->packet.srcId,self->packet.sessionId,1,data);
    }
}

static void add(f32 run_voltage, u16 run_ms){
    ASSERT(mix.is_busy, "mixing busy\r\n");
    mix.run_voltage = run_voltage;
    mix.run_ms = run_ms;
    mix.is_busy = true;
    mix.is_report = false;
    bus_os_post_sem(&mix.sem);
}


static void parse(bus_packet* packet){
    ASSERT(mix.is_busy, "mixing busy\r\n");
    mix.packet = *packet;
    u8 offset = 4;
    //无作用
    offset++;
    mix.run_voltage = ReadF32(&packet->data[offset]);
    offset+=sizeof(f32);
    mix.run_ms = ReadU32(&packet->data[offset]);
    mix.is_report = true;
    mix.is_busy = true;
    bus_os_post_sem(&mix.sem);
}

/** 局部函数 **/
static void mix_thread(void* data){
    printf("** mix thread start ***\r\n");
	while(1)
	{
		bus_os_pend_sem(&mix.sem, 0);
        run(&mix);
        mix.is_busy = false;
	}
}

