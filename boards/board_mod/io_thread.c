#include "io_mod.h"
#include "itf_00020000.h"
#include "itf_00020001.h"

#include "itf_00020010.h"
#include "itf_00020030.h"
#include "itf_00030000.h"
#include "itf_00030001.h"
#include "bus.h"
#include "stm32f4xx.h"
#include "project_cfg.h"

#if IO_CFG_FILE_NEW ==1
#include "io_thread_cfg.h"
#else
#include "io.h"
#endif


#if READ_GROUP_CNT > 0
extern Io_readable_t* readable_io[READ_GROUP_CNT];

#endif

#if WRITE_GROUP_CNT > 0
extern void writable_io_init(void );
extern Io_writable_t* writable_io[WRITE_GROUP_CNT]; 
#endif

__weak void io_hd_init(void)
{
}

void io_thread_init(void (*thread_create)(void (*thread)(void* data))){
	io_hd_init();
	
#if WRITE_GROUP_CNT > 0
	writable_io_init();
#endif
#if READ_GROUP_CNT > 0
	readable_io_init();
#endif	
#if READABLE_CHANGE_ENABLE_VALUE > 0
	void io_change_thread(void* data);
    thread_create(io_change_thread);
#endif
#if 0
    io_write_test(1);
#endif
}

void io_cmd_run(u32 itf_code, bus_packet* packet){
    switch(itf_code){
#if WRITE_GROUP_CNT > 0
        case 0x00020000:{
			Itf_00020000_t* cmd = itf_00020000_new();
            cmd->packet_parse(cmd, packet);
			printf("state=%x\r\n",cmd->content.m1_state);
            writable_io[0]->group_write(writable_io[0], cmd->content.m1_state);
			cmd->rtn_and_free(cmd, packet, 0);
            break;
        }	
        case 0x00030000:{
            //TODO::优化
            Itf_00030000_t* cmd = itf_00030000_new();
            cmd->packet_parse(cmd, packet);
            printf("id=%d\r\n", cmd->content.idx);
            printf("duty = %d\r\n", cmd->content.duty_cycle);
            writable_io[0]->io_array[cmd->content.idx].pwm_dutycycle = cmd->content.duty_cycle;
            writable_io[0]->io_write(writable_io[0], cmd->content.idx, IO_DISABLE);
            cmd->rtn_and_free(cmd, packet, 0);
            break;
        }



	  case 0x00020001:{
			Itf_00020001_t* cmd = itf_00020001_new();
            cmd->packet_parse(cmd, packet);
			
			printf("gid=%d,state=%x\r\n",cmd->content.group_idx,cmd->content.m1_state);
            writable_io[cmd->content.group_idx]->group_write(writable_io[cmd->content.group_idx], cmd->content.m1_state);
			cmd->rtn_and_free(cmd, packet, 0);
            break;
      }	
	  case 0x00030001:{
            //TODO::优化
            Itf_00030001_t* cmd = itf_00030001_new();
            cmd->packet_parse(cmd, packet);
            printf("id=%d\r\n", cmd->content.idx);
            printf("duty = %d\r\n", cmd->content.duty_cycle);
            writable_io[cmd->content.gid]->io_array[cmd->content.idx].pwm_dutycycle = cmd->content.duty_cycle;
		  
            writable_io[cmd->content.gid]->io_write(writable_io[cmd->content.gid], cmd->content.idx, IO_ENABLE);
		  
            cmd->rtn_and_free(cmd, packet, 0);
            break;
        }

		
#endif
		
#if READ_GROUP_CNT > 0
        case 0x00020010:{
            u32 state = readable_io[0]->group_read(readable_io[0]);
			printf("%x\r\n", state);
            Itf_00020010_rtn(packet, state);
            break;
        }
#endif
        default:ASSERT("1", "cmd no support");
    }
}

#if READABLE_CHANGE_ENABLE_VALUE > 0
static void io_change_thread(void* data){
    while (1)
    {
        u32 value = readable_io[0]->listen(readable_io[0]);//监听指定的IO是否翻转
        if(value){//0 不变 1 变为失能 2 变为使能
			printf("io is change:\r\n");

			printf("%x\r\n",value);
            Itf_00020030_send(value);
        }
        bus_os_sleep(1000);//IO线程挂起1000ms
#if 0
	io_read_test();
#endif
    }
}
#endif

