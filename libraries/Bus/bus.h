#ifndef BUS_H_
#define BUS_H_

#include "os_implement.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "tool.h"
#include "bus_cfg.h"

#define NONE 					"\033[m"
#define RED 					"\033[0;32;31m"
#define LIGHT_RED 				"\033[1;31m"
#define GREEN 					"\033[0;32;32m"
#define LIGHT_GREEN 			"\033[1;32m"
#define BLUE 					"\033[0;32;34m"
#define LIGHT_BLUE 				"\033[1;34m"
#define DARY_GRAY 				"\033[1;30m"
#define CYAN 					"\033[0;36m"
#define LIGHT_CYAN 				"\033[1;36m"
#define PURPLE 					"\033[0;35m"
#define LIGHT_PURPLE 			"\033[1;35m"
#define BROWN 					"\033[0;33m"
#define YELLOW 					"\033[1;33m"
#define LIGHT_GRAY 				"\033[0;37m"
#define WHITE 					"\033[1;37m"



#define BUS_RECEIVE_PACK_MAX_CNT       50       //下位机能接受的并执行的指令包的最大的数量

//Edit By fangzx   可控的打印輸出功能，区分register 和 Element 中的调试功能和真正的输出 

#define DEBUG_ENABLE
#ifdef  DEBUG_ENABLE
#define dbgprintf(fmt,...) printf((fmt),##__VA_ARGS__)
#define dbgputs(fmt,stream,length) for(int i = 0;i<(length);i++) dbgprintf(fmt,(stream)[i])
#define ASSERT_DEFEAT(Refer)          if((Refer)==0)                      \
                    dbgprintf("asser at line:%d:%s \n\tdefeat -> %s\n",   \
                    __LINE__,__FILE__,#Refer)
#define BUS_ITA_RTN(packet, data, data_len) \
		bus_ita_return(packet.adapterId, packet.srcId, packet.sessionId, data_len, data)


#else
    #define dbgprintf(fmt,...)
    #define dbgputs(fmt,stream,length)
    #define ASSERT_DEFEAT(Refer)
#endif

//End of fangzx

#define     BUS_ASSERT(exp,log) if(exp){  while(1) printf("FileName: %s  BUS e =%d,%s\r\n",__FILE__ , __LINE__ , log);    };



typedef struct  {
	u8   adapterId;
	u8   srcId;                 //nodeId 0 是保留id 不能用作地址
	u8   desId;
	u8   functionId ;
	u8   sessionId ;
	u8   data_len   ;
	u8   data[8];
	
}bus_frame;

typedef struct 
{
	u8                  adapterId;
	u8                  srcId;
	u8                  desId;
	u8                  functionId;
	u8                  sessionId;
	u32                 data_len;
	u8                  *data;

}bus_packet;

//------------ 函数类型 -----------------------------
typedef u32(*bus_ita_call)(bus_packet * packet);
typedef u32(*adapter_send_type)(void *ths, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data);	//增加的网卡之需要实现这个函数就可以了
typedef u32(*adapter_init_type)(void *ths, u8 localId);		//网卡初始化函数

//------------------------ 网卡结构 -------------------------------------------
//------------------------ 为了让协议大统一 特加此结构 任何方式只需要实现数据链路 不管是串口还是can 或者其他什么东西都能通信

typedef struct _bus_adapter
{
	u8					adatperId;		//网卡id
	u8                  localId;		//本地的id
	adapter_send_type	adapter_send;	//增加的网卡之需要实现这个函数就可以了
	adapter_init_type   adapter_init;		//网卡初始化函数
	struct _bus_adapter			*next;
}bus_adapter;

typedef void(*BUS_NEW_NODE_CALLBACK)(bus_adapter* adapter, int addr);

void * my_malloc(int size);
void bus_ita_return_unkonw_cmd(bus_packet * packet);
void my_bus_simple_send(u32 adapterId, u32 srcId, u32 desId, u32 functionId, u32 sessionId, u32 data_len, u8 *data);
//-------------- 本地网卡的建立  网卡send应该是线程安全的 --------------------------------------------------------
void bus_adapter_add(bus_adapter *adapter, adapter_send_type send, adapter_init_type init);
//--------------- 应用层初始化 ----------------------
#if BUS_USE_ROUTER_FUNC == 1
u32 bus_app_init(bus_ita_call callback,bus_ita_call broadcast, BUS_NEW_NODE_CALLBACK new_node_callback);
#else
u32 bus_app_init(bus_ita_call callback,bus_ita_call broadcast);
#endif

//--------------- 下面的函数是各模块实现功能的最常用函数 ita 为 interactive 的缩写 发送之后无返回则一直阻塞
//u32 bus_ita_send(u32 adapterId,u32 desId,u32 data_len,u8 *data,u8 *dataout,u32 *dataoutLen,u32 timeOut);
//--------------- 心跳保持的发送函数 ----------------------------------------------------------------------------
u32 bus_ita_hb_send(u32 adapterId, u32 desId, u32 data_len, u8 *data, u8 *dataout, u32 *dataoutLen);

//发送数据 发送完毕一直等待数据返回 或者心跳维持
u32 bus_ita_return(u32 adapterId,u32 desId,u8 ita_session,u32 data_len,u8 *data);
void bus_rx_packet_erase(u32 adapterId, u32 desId, u8 ita_session);
u32 bus_ita_broadcast(u32 adapterId,u32 data_len, u8 *data);	//广播的函数


//--------------- 下面是给驱动调用的函数  此函数不是线程安全的 调用者必须保证其线程安全  ----------------------
void bus_for_driver_call(u32 adapterId, u32 srcId, u32 desId, u32 functionId, u32 sessionId, u32 data_len, u8 *data);
bool bus_is_init_ok(void);

void my_bus_for_driver_call(u32 adapterId, u32 srcId, u32 desId, u32 functionId, u32 sessionId, u32 data_len, u8 *data);

#if BUS_USE_ROUTER_FUNC == 1


	typedef struct router_map_struct
	{
		bus_adapter 			*adapter;
		u8               			id;
		struct router_map_struct *next;
	}router_map_struct;

	router_map_struct* New_router_map_struct(void);	
	void router_list_add(router_map_struct *rt);


#endif







#endif
 
