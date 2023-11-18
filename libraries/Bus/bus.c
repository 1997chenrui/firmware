#include "bus.h"
#include "data_struct.h"
#include "bus_cfg.h"
#include "ErrCode.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


//------------------- 下面是map  的模板 ----------------------------

#define   MAP_CREATE(cnt,keyT,valueT ,name ,container )			                                                \
                        void name##_insert(keyT *key, valueT *value)					\
						{																						\
                            CPU_SR_ALLOC(); /*进入临界段*/                           \
                            OS_ENTER_CRITICAL();/*关中断*/                                            \
                            for (int i = 0; i<cnt; i++)															\
							{																					\
                                if (!(container)[i].isUsed)											\
								{																				\
                                    (container)[i].isUsed = true;										\
                                    (container)[i].key = *key;										\
                                    (container)[i].value = *value;									\
                                    OS_EXIT_CRITICAL();        \
                                    return;																		\
								}																				\
							}																					\
							OS_EXIT_CRITICAL();        \
							BUS_ASSERT(true, "no map space");													\
						}																						\
						void name##_modify(keyT *key, valueT *value)								\
						{																						\
                            CPU_SR_ALLOC();                            \
                            OS_ENTER_CRITICAL();                                            \
                            for (int i = 0; i < cnt; i++)															\
							{																					\
                                if ((container)[i].isUsed)											\
								{																				\
                                    if (name##_key_compare(key, &(container)[i].key) == 0)			\
									{																			\
                                        (container)[i].value = *value;\
										OS_EXIT_CRITICAL(); \
										return;\
									}																			\
								}																				\
							}																					\
                            OS_EXIT_CRITICAL(); \
							BUS_ASSERT(true, "no this key");\
						}																								\
						valueT * name##_serach(keyT *key)								\
						{																						\
                            CPU_SR_ALLOC();                            \
                            OS_ENTER_CRITICAL();                                            \
                            for (int i = 0; i < cnt; i++)															\
							{																					\
                                if ((container)[i].isUsed)											\
								{																				\
                                    if (name##_key_compare(key, &(container)[i].key) == 0)			\
									{																			\
                                        OS_EXIT_CRITICAL(); \
                                        return &(container)[i].value;									\
									}																			\
								}																				\
							}																					\
                            OS_EXIT_CRITICAL(); \
							return NULL;																		\
						}																						\
						void name##_del(keyT *key)												\
						{																						\
                            CPU_SR_ALLOC();                            \
                            OS_ENTER_CRITICAL();                                            \
                            for (int i = 0; i < cnt; i++)															\
							{																					\
                                if ((container)[i].isUsed)											\
								{																				\
                                    if (name##_key_compare(key, &(container)[i].key) == 0)			\
									{																			\
                                        (container)[i].isUsed = false;								\
                                        OS_EXIT_CRITICAL(); \
                                        return;																	\
									}																			\
								}																				\
							}																					\
							OS_EXIT_CRITICAL();        \
							/*BUS_ASSERT(true, "no ths key");*/														\
						}

#define   AGE_MAP_CREATE(cnt,keyT,valueT ,name ,container,timeThresholds)			                                            \
                        void name##_insert(keyT *key, valueT *value)											\
						{	u32 mTimeStamp	= bus_os_time_get();												\
                            int oldest_idx = -1;                                                                \
                            CPU_SR_ALLOC();                            											\
                            OS_ENTER_CRITICAL();                                            					\
                            for (int i = 0; i<cnt; i++)															\
							{																					\
								if((container)[i].age == 0)                                 					\
								{																				\
                                    (container)[i].key = *key;											\
                                    (container)[i].value = *value;										\
                                    (container)[i].error = false;										\
									(container)[i].age = 1;												\
									(container)[i].timeStamp = mTimeStamp;										\
                                    OS_EXIT_CRITICAL();        											\
                                    return;																		\
								}else if((container)[i].age>1)                   							\
                                {                                      \
                                    if(oldest_idx==-1) oldest_idx=i;\
                                    else oldest_idx=((container)[i].age>(container)[oldest_idx].age)?i:oldest_idx; \
                                }\
                            }																					\
							if(oldest_idx == -1)																\
							{																					\
								BUS_ASSERT(true, "1 no map space");                                             \
							}																					\
							else{(container)[oldest_idx].age = 1;			                                    \
							(container)[oldest_idx].error = false;												\
							(container)[oldest_idx].key = *key;(container)[oldest_idx].value = *value;			\
							(container)[oldest_idx].timeStamp = mTimeStamp;}									\
							OS_EXIT_CRITICAL();																	\
						}																						\
						valueT * name##_serach(keyT *key)														\
						{																						\
                            CPU_SR_ALLOC();                            											\
                            OS_ENTER_CRITICAL();                                            					\
                            for (int i = 0; i < cnt; i++)														\
							{																					\
								/*if(!((container)[i].isUsed == 0 && (container)[i].age == 0))*/				\
								if((container)[i].age >= 1)                                 /* wzc 20201203 */	\
								{																				\
                                    if (name##_key_compare(key, &(container)[i].key) == 0)						\
									{																			\
                                        OS_EXIT_CRITICAL(); 													\
                                        return &(container)[i].value;											\
									}																			\
								}																				\
							}																					\
                            OS_EXIT_CRITICAL();																	\
							return NULL;																		\
						}																						\
						bool name##_error(keyT *key)														\
						{																						\
                            CPU_SR_ALLOC();                            											\
                            OS_ENTER_CRITICAL();                                            					\
                            for (int i = 0; i < cnt; i++)														\
							{																					\
								if (name##_key_compare(key, &(container)[i].key) == 0)							\
								{																				\
									OS_EXIT_CRITICAL(); 														\
									return (container)[i].error;													\
								}																				\
							}																					\
                            OS_EXIT_CRITICAL();																	\
							return true;																			\
						}																						\
						void name##_del(keyT *key, bool error)													\
						{																						\
                            CPU_SR_ALLOC();                            											\
                            OS_ENTER_CRITICAL();                                            					\
                            for (int i = 0; i < cnt; i++)														\
							{																					\
								if((container)[i].age >= 1)							                        \
								{																				\
                                    if (name##_key_compare(key, &(container)[i].key) == 0)						\
									{																			\
										(container)[i].age++;													\
										(container)[i].error = error;											\
										OS_EXIT_CRITICAL(); 													\
                                        /*return;*/																	\
									}																			\
								}																				\
							}																					\
							OS_EXIT_CRITICAL();                                                                 \
							/*BUS_ASSERT(true, "no ths key");	*/													\
						}																						\
						void name##_older()																		\
						{																						\
                            CPU_SR_ALLOC();                            											\
                            OS_ENTER_CRITICAL();                                            					\
                            for (int i = 0; i<cnt; i++)															\
							{																					\
                                /*if (NODE_USED((container)[i].isUsed, (container)[i].age))*/					\
								if((container)[i].age > 1)                            /* wzc 20201203 */		\
								{																				\
									(container)[i].age++;														\
								}																				\
							}																					\
							OS_EXIT_CRITICAL();        															\
						}																						\
						void name##_del_outdated()																\
						{																						\
							u32 mTimeStamp = bus_os_time_get();													\
							CPU_SR_ALLOC();                            											\
                        	OS_ENTER_CRITICAL();																\
							for (int i = 0; i < cnt; i++)														\
							{																					\
                                /*if (NODE_USED((container)[i].isUsed, (container)[i].age))*/					\
								if((container)[i].age > 1)                            /* wzc 20201203 */		\
								{																				\
                                    int timeSpan = mTimeStamp > (container)[i].timeStamp?mTimeStamp - 		\
                                    (container)[i].timeStamp:0xffffffff+mTimeStamp-(container)[i].timeStamp;\
                                    if(timeSpan > timeThresholds)										    \
                                    {																		\
										(container)[i].error = false;								\
                                        (container)[i].age = 0;												\
                                    }																		\
								}																				\
							}																					\
							OS_EXIT_CRITICAL(); 																\
						}



//============================== 队列 模板 ========================================================================


#define QUEUE_CREATE(name,container,eleT,QueueSize)															\
						bool name##_Enter(eleT *data)										\
						{																	\
							if (container.ele_size >= QueueSize)								\
							{																\
								return false;												\
							}																\
							else															\
							{																\
								CPU_SR_ALLOC();												\
								OS_ENTER_CRITICAL();										\
								container.element[container.w_idx] = *data;				\
								container.w_idx = (container.w_idx + 1) % QueueSize;					\
								container.ele_size++;												\
								name##_post();													\
								OS_EXIT_CRITICAL();											\
								return true;												\
							}																\
						}																	\
						bool name##_Delete(eleT *data)									\
						{																	\
							if (container.ele_size <= 0)											\
							{																\
								return false;												\
							}																\
							else															\
							{																\
								CPU_SR_ALLOC();												\
								OS_ENTER_CRITICAL();										\
								*data = container.element[container.r_idx];									\
								container.r_idx = (container.r_idx + 1) % QueueSize;					\
								container.ele_size--;												\
								OS_EXIT_CRITICAL();											\
																							\
								return true;												\
							}																\
						}																	\
						bool name##_Waite(void)											\
						{																	\
							return name##_pend();												\
						}																	\
						int name##_GetSize(void)											\
						{																	\
							return container.ele_size;												\
						}


/*************************************** 网络模型 *****************************************************

	现在全部改成c语言的东西，力求做到最简单最稳定

    帧格式 :
        头帧 :  0-1  帧索引
                2-3  包格式
                4-7  包CRC32

        其他帧: 0-1  帧索引
                2-7  内容


*******************************************************************************************************/




#define  					TASK_RX_PRIO                           1u

#ifndef  TASK_RX_STK_SIZE_EXTER 
#define  					TASK_RX_STK_SIZE                     	512u
#else 
#define  					TASK_RX_STK_SIZE                     	TASK_RX_STK_SIZE_EXTER
#endif
static  bus_os_thread   	TASK_RX_TCB;
static  u32  				TASK_RX_Stk[TASK_RX_STK_SIZE];




#define  					TASK_CALLBACK_PRIO                           9u

#ifndef  TASK_CALLBACK_STK_SIZE_EXTER 

//callback也创建一个线程
#define  					TASK_CALLBACK_STK_SIZE                     	256u

#else 
#define  					TASK_CALLBACK_STK_SIZE                     	TASK_CALLBACK_STK_SIZE_EXTER
#endif

static  bus_os_thread   	TASK_CALLBACK_TCB;
static  u32  				TASK_CALLBACK_Stk[TASK_CALLBACK_STK_SIZE];


static  bus_ita_call bus_rx_callback;
static  bus_ita_call bus_rx_broadcast;
#if BUS_USE_ROUTER_FUNC == 1
static  BUS_NEW_NODE_CALLBACK bus_new_note_callback;
#endif

static bus_os_sem  session_id_mutex_sem;
static  u8 session_id_poll[256] = {0};
static volatile  u8                  bus_session_id;

typedef enum{
	SESSION_UNUSE = 0,
	SESSION_USING = 1
}Session_State;
//sessionId 管理

static int create_session_id()
{
	u8 now_id = bus_session_id;
	bus_os_pend_sem(&session_id_mutex_sem, 0);
	do{
		bus_session_id = bus_session_id == 255 ? 0 : bus_session_id + 1;
		if(now_id == bus_session_id) 
			return -1;
	}while(session_id_poll[bus_session_id] != SESSION_UNUSE);

	session_id_poll[bus_session_id] = SESSION_USING;
	
	bus_os_post_sem(&session_id_mutex_sem);

	
	printf("cr sid=%d\r\n",bus_session_id);
	return bus_session_id;
}

static void destroy_session_id(u8 sessionId)
{
	session_id_poll[bus_session_id] = SESSION_UNUSE;
//	printf("ds sid=%d\r\n",sessionId);
}

//------------------------------------- bus 的内存管理 ---------------------------------
volatile int MallocTest = 0;
void* my_malloc(int size)
{
	void* p = malloc(size);
    if(p == NULL)
    {
        int cnt = 0;
        while(1)
        {
            if(malloc(8) != NULL)
            {
                cnt++;
            }else
            {
                while(1) printf("my_malloc error mf=%d,cnt=%d\r\n",__LINE__,cnt);
            }
        }
    }
    
//	if(p == NULL)
//	{
//		while(1)
//        {
//            printf("my_malloc error\r\n");
//        }
//	}
	memset(p, 0, size);
	MallocTest++;
	return p;
}

void my_free(void* p)
{
	MallocTest--;
	free(p);
}


//-------------------------------------------- 引用计数垃圾回收数组 ---------------------------------------------------------------


typedef  struct
{
	volatile int cnt;
	volatile int ref;
	volatile unsigned char* data;
//    u32 crc;
} bus_packet_base_t;


typedef struct
{
	volatile bus_packet_base_t* base;
} bus_packet_t;


bus_packet_t  bus_packet_alloc(int cnt)
{
	bus_packet_t pack;
	pack.base = (bus_packet_base_t*)my_malloc(sizeof(bus_packet_base_t));
	pack.base->cnt = cnt;
	pack.base->ref = 1;
	pack.base->data = (unsigned char*)my_malloc(cnt);
	return pack;
}

bus_packet_t  bus_packet_copy(bus_packet_t r)
{
	bus_packet_t l = r;
	l.base->ref++;
	return l;
}

void bus_packet_del(bus_packet_t*  p)
{
	p->base->ref--;
	if(p->base->ref == 0)
	{
		my_free((void*)p->base->data);
		p->base->data = NULL;
		my_free((void*)p->base);
        p->base = NULL;
	}
}


//为了避免进入中断耗时太多 影响电机 再搞个队列 让can中断收到数据就插进去一个简单的队列

static bus_os_sem        frame_q_sem;
static bool frame_q_post(void)
{
	u32 code = bus_os_post_sem(&frame_q_sem);
	return code == 0;
}

static bool frame_q_pend(void)
{
	u32 code = bus_os_pend_sem(&frame_q_sem, 0);
	return code == 0;
}


static bus_os_sem      packet_q_sem;

static bool packet_q_pend(void)
{
	u32 code = bus_os_pend_sem(&packet_q_sem,0);
	return code == 0;
}

// static volatile int pack_ct_cnt = 0;

static bool packet_q_post(void)
{
	u32 code = bus_os_post_sem(&packet_q_sem);
	return code == 0;
}


//---------------- 静态方法声明 -----------------------
typedef struct
{
	union
	{
		struct
		{
			u8 adapterId;
			u8 srcId;
			u8 sessionId;
			u8 functionId;
		};
		u32  key_compare;
	};

} expect_packet_key_t;

typedef struct
{
	int                      pre_fm_idx;        //上一个帧的索引，防止中间丢包
	bus_packet_t               pack;          //包引用
	bus_os_sem*               sem;
} expect_packet_value_t;


static void expect_packet_del(expect_packet_key_t* key, expect_packet_value_t* value, u8* dataOut, u32* dataOutLen);
static void bus_simple_send(u32 adapterId, u32 desId, u8 functionId, u8 sessionId, u32 data_len, u8* data);

typedef struct // 用来防止重复发送包的时候重传的时候 接收方多次收到
{
	union
	{
		struct
		{
			u8 functionId;
			u8 sessionId;
			u8 adatperId;
			u8 srcId;
		};
		u32  compare;
	};
} bus_frame_id_t;


//其他节点对本机ack的一个map
typedef struct
{
	union
	{
		struct
		{
			u8  functionId;
			u8  sessionId;
			u8  adatperId;
			u8  srcId;
		};
		u32 key_compare;
	};

} ack_map_key_t;

typedef struct
{
	bus_os_sem* sem;
} ack_map_value_t;




#ifndef ACK_MAP_MAX_CNT_EXTERN
#define ACK_MAP_MAX_CNT     50     //
#else
#define ACK_MAP_MAX_CNT     ACK_MAP_MAX_CNT_EXTERN     //
#endif


#ifndef PACK_MAP_MAX_CNT_EXTERN
#define PACK_MAP_MAX_CNT     50     //
#else
#define PACK_MAP_MAX_CNT     PACK_MAP_MAX_CNT_EXTERN     //
#endif


typedef struct
{
	struct
	{
		ack_map_key_t key;
		ack_map_value_t value;
		bool isUsed;
	} container[ACK_MAP_MAX_CNT];

	bus_os_sem ack_map_mutex;	//对ack的保护信号量
} AckMap_t;

#define ACK_MAP_PEND()  {	bus_os_pend_sem(&Bus.ack_map.ack_map_mutex, 0)
#define ACK_MAP_POST()	    bus_os_post_sem(&Bus.ack_map.ack_map_mutex); }

typedef struct
{
	u8   adapterId;
	u8   srcId;                 //nodeId 0 是保留id 不能用作地址
	u8   desId;
	u8   functionId ;
	u8   sessionId ;
	bus_packet_t   pack;
} packet_q_t;

typedef struct
{
	AckMap_t  ack_map;

	struct
	{
		expect_packet_key_t key;
		expect_packet_value_t value;
		bool isUsed;
	} pack_container[PACK_MAP_MAX_CNT];

	//定义包队列容器
	struct
	{
#if defined(STM32F103xB)
		packet_q_t element[2<<5];
#elif defined(STM32F407xx)
		packet_q_t element[100];
#endif
		volatile unsigned short r_idx;
		volatile unsigned short w_idx;
		volatile int ele_size;
	} packet_q;

	// 定义帧队列容器
	struct
	{
#if defined(STM32F103xB)
		bus_frame element[2<<5];
#elif defined(STM32F407xx)
	#if (BUS_USE_ROUTER_FUNC == 1)
		bus_frame element[640];
	#else
		bus_frame element[100];
	#endif
#endif

		volatile unsigned short r_idx;
		volatile unsigned short w_idx;
		volatile int ele_size;
	} frame_q;

	//定义接收包生命周期 map，一旦包建立成功，在周期内 全部忽略重传 否则巨大隐患
	struct
	{
		bus_frame_id_t key;
		int value;
		bool error;
		int age;    //0未使用 1使用中 >1年龄
		u32 timeStamp;
	} rx_p_container[PACK_MAP_MAX_CNT];

	//发送包map
	struct
	{
		bus_frame_id_t key;
		int value;
		bool isUsed;
	} tx_p_container[PACK_MAP_MAX_CNT];
} Bus_t;


  Bus_t Bus;


static int ack_map_key_compare(void* l, void* r)
{
	ack_map_key_t* ll = (ack_map_key_t*)l;
	ack_map_key_t* rr = (ack_map_key_t*)r;
	if(ll->key_compare < rr->key_compare)
	{
		return -1;
	}
	else if(ll->key_compare == rr->key_compare)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}

static int rx_p_map_key_compare(void* l, void* r)
{
	bus_frame_id_t* ll = (bus_frame_id_t*)l;
	bus_frame_id_t* rr = (bus_frame_id_t*)r;
	if(ll->compare < rr->compare)
	{
		return -1;
	}
	else if(ll->compare == rr->compare)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


static int tx_p_map_key_compare(void* l, void* r)
{
	bus_frame_id_t* ll = (bus_frame_id_t*)l;
	bus_frame_id_t* rr = (bus_frame_id_t*)r;
	if(ll->compare < rr->compare)
	{
		return -1;
	}
	else if(ll->compare == rr->compare)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

//期望包 key 比较函数
static int pack_map_key_compare(void* l, void* r)
{
	expect_packet_key_t* ll = (expect_packet_key_t*)l;
	expect_packet_key_t* rr = (expect_packet_key_t*)r;

	if(ll->key_compare < rr->key_compare)
	{
		return -1;
	}
	else if(ll->key_compare == rr->key_compare)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

MAP_CREATE(ACK_MAP_MAX_CNT,ack_map_key_t,ack_map_value_t,ack_map,Bus.ack_map.container)	;
MAP_CREATE(PACK_MAP_MAX_CNT,expect_packet_key_t,expect_packet_value_t,pack_map,Bus.pack_container)	;
AGE_MAP_CREATE(PACK_MAP_MAX_CNT,bus_frame_id_t,int,rx_p_map,Bus.rx_p_container,5000);
MAP_CREATE(PACK_MAP_MAX_CNT,bus_frame_id_t,int,tx_p_map,Bus.tx_p_container);


#if defined(STM32F103xB)
	QUEUE_CREATE(packet_q,Bus.packet_q, packet_q_t,2<<5);
	QUEUE_CREATE(frame_q,Bus.frame_q,bus_frame,2<<5);
#elif defined(STM32F407xx)
	QUEUE_CREATE(packet_q,Bus.packet_q, packet_q_t,100);
#if (BUS_USE_ROUTER_FUNC == 1)
	QUEUE_CREATE(frame_q,Bus.frame_q,bus_frame,640);
#else
	QUEUE_CREATE(frame_q,Bus.frame_q,bus_frame,100);
#endif
#endif


static void ack_map_set(u8  adatperId, u8 srcId, u8 sessionId,u8 functionId, bus_os_sem* sem)//
{
	ack_map_key_t key;
	key.adatperId = adatperId;
	key.srcId = srcId;
	key.sessionId = sessionId;
	key.functionId = functionId;

	ack_map_value_t		value;
	value.sem = sem;

	ACK_MAP_PEND();
	ack_map_insert(&key, &value);
	ACK_MAP_POST();
}
static void ack_map_post(u8  adatperId,u8 srcId, u8 sessionId,u8 functionId)
{
	ACK_MAP_PEND();

	ack_map_key_t key;
	key.adatperId = adatperId;
	key.srcId = srcId;
	key.sessionId = sessionId;
	key.functionId = functionId;

	ack_map_value_t* res = NULL;

	res = ack_map_serach(&key);
	if(res != NULL)
	{
		bus_os_post_sem(res->sem);
	}
	else
	{
        //此处是对方多发出的ack信号,正常
		BUS_LOG("snim i=%d,s=%d,f=%d\r\n",srcId,sessionId,functionId);
	}
	ACK_MAP_POST();

}


static void ack_map_erase(u8  adatperId, u8 srcId, u8 sessionId, u8 functionId)
{
	ack_map_key_t key;
	key.adatperId = adatperId;
	key.srcId = srcId;
	key.sessionId = sessionId;
	key.functionId = functionId;

	ACK_MAP_PEND();
	ack_map_del(&key);
	ACK_MAP_POST();
}


typedef enum
{
	BUS_FUNC_SEND = 1,					//用于发送方给接收方传送数据
	BUS_FUNC_SEND_ACK = 2,				//用于数据发送过去的确认回复

	BUS_FUNC_RETURN = 3,				//用于指令发过去 数据的返回
	BUS_FUNC_RET_ACK = 4,				//专门对return数据的返回

	BUS_FUNC_HEART_BEAT = 5,			//用于发送心跳	等待retur 超时之后就发送心跳来进行维护，心跳超时2次算作连接断开
	BUS_FUNC_HEART_BEAT_ACK = 6,		//专门对心跳数据的应答

	BUS_FUNC_HEART_BEAT_ERR_ACK = 7, // 接收方: 接收到心跳后, 发现错误,如生命周期已结束, 给对方一个err ack

} BUS_PACK_FUNC;

static bus_os_sem  expect_packet_tree_mtx;
static int         expect_packet_tree_mutex_cnt = 0;

static int err_func_id_convert_to(int err_id)
{
	return err_id == BUS_FUNC_HEART_BEAT_ERR_ACK ? BUS_FUNC_HEART_BEAT_ACK : err_id;  
}

static void        expect_packet_tree_pend(void)
{
	if(expect_packet_tree_mutex_cnt == 0)
	{
		bus_os_pend_sem(&expect_packet_tree_mtx, 0);
	}
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	expect_packet_tree_mutex_cnt++;
	OS_EXIT_CRITICAL();

}

static void expect_packet_tree_post(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	expect_packet_tree_mutex_cnt--;
	OS_EXIT_CRITICAL();
	if(expect_packet_tree_mutex_cnt == 0)
	{
		bus_os_post_sem(&expect_packet_tree_mtx);
	}
}

//定义信息树的 设置，插入，删除 函数

static void expect_packet_set(expect_packet_key_t* key, bus_os_sem* sem)
{
	expect_packet_value_t value;
	value.pre_fm_idx = -1;
	value.sem = sem;
	//BUS_PACK_SET_NULL(value.pack);
	value.pack.base = NULL;
	expect_packet_tree_pend();
	pack_map_insert(key,&value);
	expect_packet_tree_post();
}

static void expect_packet_insert(bus_frame* frame)
{
	bus_frame_id_t send_frame_id;
	send_frame_id.adatperId = frame->adapterId;
	send_frame_id.srcId = frame->srcId;
	send_frame_id.sessionId = frame->sessionId;
	send_frame_id.functionId = BUS_FUNC_SEND;
	//如果是收到了心跳包，那么就直接回复一个ACK就可以了
	if(frame->functionId == BUS_FUNC_HEART_BEAT)
	{
		int func_id = BUS_FUNC_HEART_BEAT_ACK;
		// 生命周期已经进入接收完毕时期, 还能接收到心跳信号, 直接发送错误ack就好
		if(rx_p_map_error(&send_frame_id))
		{ 
			printf("re hb compare=%d not alive\r\n,",send_frame_id.compare);
			func_id = BUS_FUNC_HEART_BEAT_ERR_ACK;
		}
		bus_simple_send(frame->adapterId, frame->srcId, func_id, frame->sessionId, 0, NULL);
		return;
	}

	rx_p_map_del_outdated();
	expect_packet_tree_pend();
	expect_packet_key_t pack_key;
	pack_key.adapterId = frame->adapterId;
	pack_key.srcId = frame->srcId;
	pack_key.sessionId = frame->sessionId;
	pack_key.functionId = frame->functionId;
	bus_frame_id_t frame_id;
	frame_id.adatperId = frame->adapterId;
	frame_id.srcId = frame->srcId;
	frame_id.sessionId = frame->sessionId;
	frame_id.functionId = frame->functionId;

	expect_packet_value_t* res = pack_map_serach(&pack_key);
	if(frame->functionId == BUS_FUNC_RETURN)	//是回复包说明之前肯定预设好了等你回复
	{
		//下面去期望信息包树里面去找找看
		if(res == NULL)
		{
			printf("expect packe not found\r\n");
			//对于回复包 期望树里面肯定存在 不存在的要直接忽略掉 啥都不能干
            //报错后如心跳超时/ack超时后, 对方才将数据传输上来, 此时的数据直接忽略, 正常.
            //成功处理了数据, 并下发了ack, 但对方没有收到, 仍在重发
            //此处给个回复, 虽然对方会多收到ack信号, 但仍然好过对方一直重发
            int frame_idx = ReadU16(frame->data);
			int* packLen = tx_p_map_serach(&send_frame_id);
			if(packLen != NULL && *packLen != -1)
			{
				if(*packLen == (frame_idx-1)*6 + frame->data_len-2)
				{
					bus_simple_send(frame->adapterId, frame->srcId, BUS_FUNC_RET_ACK, frame->sessionId, 0, NULL);
					printf("rt not exist dId=%d,fId=%d,seId=%d,fId=%d",frame->desId,frame->functionId,frame->sessionId,frame_idx);
				}
			}
			goto END;
		}
		else
		{
			int frame_idx = ReadU16(frame->data);

			if(frame_idx == 0)		//是头帧
			{
				int pack_len = ReadU32(frame->data + 2);    //包长度设置
				tx_p_map_modify(&send_frame_id, &pack_len);
				BUS_ASSERT(pack_len==0,"rpak len 0");
//                BUS_ASSERT(res->pack.base != NULL,"pack occupy");
				//if(! BUS_PACK_IS_NULL(res->pack))
				if(res->pack.base != NULL)
				{
                    //说明在重发
					//空才创建，不空直接就删除掉
					//BUS_PACK_COPY_CONSTRUCT(pack,res->pack);
					{
						bus_packet_t pack = res->pack;
						//BUS_PACK_DEL(pack);
						bus_packet_del(&pack);
					}
				}

				//BUS_PACK_ALLLOC(pack,pack_len);
				{
					BUS_ASSERT(pack_len>1024,"pack too big");
					bus_packet_t pack = bus_packet_alloc(pack_len);  //; name.base->crc = crc
					//BUS_PACK_COPY(res->pack,pack);  //把包拷贝到map里面去
					res->pack = bus_packet_copy(pack);
					//BUS_PACK_DEL(pack);
					bus_packet_del(&pack);
				}
				res->pre_fm_idx = 0;
			}
			else
			{
				// 首先比较下有没有丢帧 ，如果这个帧索引比上次的不止大一个 那么就要考虑丢帧的处理情况
				if(res->pre_fm_idx == frame_idx - 1)
				{
					res->pre_fm_idx = frame_idx;
					//说明没有丢帧就要继续接帧
					int pack_idx = (frame_idx - 1)*6;
					//把帧拷贝到包里面去
					//BUS_PACK_MOVE_DATA(res->pack,pack_idx,frame->data + 2,frame->data_len - 2);
					for(u8 i = 0; i < frame->data_len - 2; i++)
					{
						res->pack.base->data[pack_idx + i] = frame->data[i + 2];
					}

					int packLen = 0;
					//BUS_PACK_GET_DATA_LEN(packLen,res->pack);
					BUS_ASSERT(res->pack.base == NULL,"nul ptr");
					packLen = res->pack.base->cnt;
					//看看是不是最后一帧
					if(packLen == (frame_idx - 1) * 6 + frame->data_len - 2)
					{
						//插进去
						bus_os_post_sem(res->sem);
                        //printf("rx : srcId = %d, funId = %d, sesId = %d\r\n", frame->srcId, frame->functionId, frame->sessionId);
						//给一个 return ack
						bus_simple_send(frame->adapterId, frame->srcId, BUS_FUNC_RET_ACK, frame->sessionId, 0, NULL);
					}
				}
				else
				{
					//TODO:处理之前的丢帧
					BUS_LOG("lost fmidx=%d \r\n",frame_idx);
				}
			}
		}
	}
	else if(frame->functionId == BUS_FUNC_SEND)
	{
		int frame_idx = ReadU16(frame->data);
		//首先要看是不是已经被接收了但是还没有被return  如果是这样要全部忽略 这里要必须注意
		if(res != NULL)
		{
			if(frame->desId != 0xff)
			{
				//生命周期已经到接收完毕阶段, 表明后面来的数据是重发数据
				if(rx_p_map_serach(&frame_id) != NULL)
				{
					//是最后一帧  返回一个ack
					int packLen = 0;
					//BUS_PACK_GET_DATA_LEN(packLen,res->pack);
					BUS_ASSERT(res->pack.base == NULL,"nul ptr");
					packLen = res->pack.base->cnt;
					//看看是不是最后一帧
					if(packLen == (frame_idx - 1) * 6 + frame->data_len - 2)
					{
						bus_simple_send(frame->adapterId, frame->srcId, BUS_FUNC_SEND_ACK, frame->sessionId, 0, NULL);
						BUS_LOG("ig re1\r\n");
						goto END;
					}
				}
			}
		}
		else //没找到就要创建包节点压进去
		{
			if(frame->desId != 0xff)
			{
				int* packLen = rx_p_map_serach(&frame_id);
				//数据已被删除,但生命周期仍然存在,有可能是在重发过程中,已经执行完毕,此时直接忽略就好
				if(packLen != NULL)
				{
					if(*packLen == (frame_idx-1)*6 + frame->data_len - 2)
					{
						bus_simple_send(frame->adapterId, frame->srcId, BUS_FUNC_SEND_ACK, frame->sessionId, 0, NULL);
						printf("ig re2\r\n");
					}
					goto END;
				}
			}

			//是头帧就塞一个进去，不是头帧 就直接忽略
			if(frame_idx == 0)
			{
				// printf("f sid %d\r\n", pack_key.sessionId);
				expect_packet_set(&pack_key, NULL); //压一个包节点进去
				res = pack_map_serach(&pack_key);
			}
			else
			{
				printf("can't find but not head\r\n");
				goto END;
			}
		}
		//找到了包节点
		if(frame_idx == 0)		//是头帧
		{
			int pack_len = ReadU32(frame->data + 2);    //包长度设置
			res->pre_fm_idx = 0;
			BUS_ASSERT(pack_len==0,"pak len 0");
            
//            BUS_ASSERT(res->pack.base != NULL,"pack occupy");
			//if(! BUS_PACK_IS_NULL(res->pack))
			if(res->pack.base != NULL)
			{

				printf("war:del pre pack not safe\r\n");
				//空才创建，不空直接就删除掉
				//BUS_PACK_COPY_CONSTRUCT(pack,res->pack);
				{
					bus_packet_t pack = res->pack;
					//BUS_PACK_DEL(pack);
					bus_packet_del(&pack);
				}
			}
//			BUS_PACK_ALLLOC(pack,pack_len);

			{
				BUS_ASSERT(pack_len > 1024,"pack too big");
				bus_packet_t pack = bus_packet_alloc(pack_len);  //; name.base->crc = crc
				//BUS_PACK_COPY(res->pack,pack);  //把包拷贝到map里面去
				res->pack = bus_packet_copy(pack);
				//BUS_PACK_DEL(pack);
				bus_packet_del(&pack);
			}
		}
		else
		{
			//为了防止头帧丢失，所以要先判断
			// 首先比较下有没有丢帧 ，如果这个帧索引比上次的不止大一个 那么就要考虑丢帧的处理情况
			if(res->pre_fm_idx == frame_idx - 1)
			{
				res->pre_fm_idx = frame_idx;
				int pack_idx = (frame_idx - 1)*6;
				//把帧拷贝到包里面去
				int packLen = 0;
				//BUS_PACK_GET_DATA_LEN(packLen,res->pack);
				BUS_ASSERT(res->pack.base == NULL,"nul ptr");
				packLen = res->pack.base->cnt;
				//BUS_PACK_MOVE_DATA(res->pack,pack_idx,frame->data + 2,frame->data_len - 2);
				for(u8 i = 0; i < frame->data_len - 2; i++)
				{
					res->pack.base->data[pack_idx + i] = frame->data[i + 2];
				}

				//看看是不是最后一帧
				if(packLen == (frame_idx - 1) * 6 + frame->data_len - 2)
				{
					if(frame->desId != 0xff)
					{
						//收到了数据包 返回一个ack
						bus_simple_send(frame->adapterId, frame->srcId, BUS_FUNC_SEND_ACK, frame->sessionId, 0, NULL);
					}

					//压入到packet回调队列
					//遍历结点树取出数据
					//BUS_PACK_COPY_CONSTRUCT(pack,res->pack);
					{
						bus_packet_t pack = res->pack;
						packet_q_t p;
						//BUS_PACK_COPY(p.pack,pack);
						p.pack = bus_packet_copy(pack);
						p.adapterId = frame->adapterId;
						p.srcId = frame->srcId;
						p.desId = frame->desId;
						p.sessionId = frame->sessionId;
						p.functionId = frame->functionId;
						pack_map_del(&pack_key);    //特别重要注意删除
						bus_packet_del(&pack);
                        
                        packet_q_Enter(&p);

						//rx_life:数据接收完毕,管理接收包生命周期
						rx_p_map_insert(&frame_id,&packLen);
						rx_p_map_older();
						//BUS_PACK_DEL(pack);
					}
				}
			}
			else
			{
				// TODO:处理之前的丢帧
				BUS_LOG("ls fidx=%d,idx=%d \r\n",res->pre_fm_idx,frame_idx);
			}
		}
	}
	else
	{
		BUS_LOG("not the expect functionID = %d \r\n",frame->functionId);
	}
END:
	expect_packet_tree_post();
}


static void expect_packet_del(expect_packet_key_t* key, expect_packet_value_t* value, u8* dataOut, u32* dataOutLen)
{
	expect_packet_tree_pend();

	value = pack_map_serach(key);
	BUS_ASSERT(value==NULL,"");

	//if(!BUS_PACK_IS_NULL(value->pack))
	if(value->pack.base != NULL)
	{
		//BUS_PACK_COPY_CONSTRUCT(pack,value->pack);
		{
			bus_packet_t pack = value->pack;
			BUS_ASSERT(dataOut==NULL,"");
			BUS_ASSERT(dataOutLen==NULL,"");
			int packLen = 0;
			//BUS_PACK_GET_DATA_LEN(packLen,pack);
			BUS_ASSERT(pack.base == NULL,"nul ptr");
			packLen = pack.base->cnt;
			for(int i=0; i<packLen; i++)
			{
				dataOut[i] =  pack.base->data[i];
			}
			//BUS_PACK_DEL(pack);
			bus_packet_del(&pack);
		}
	}

	pack_map_del(key);
	expect_packet_tree_post();
}

static bus_adapter*    adapterList = NULL;	//网卡链表



//----- 发送队列相关 ----------------
static bus_os_sem  tx_mutex_sem;
static bool isinitOk = false;


#if BUS_USE_ROUTER_FUNC == 1

static router_map_struct* router_list = NULL;
router_map_struct* New_router_map_struct(void)
{
	router_map_struct* rt = my_malloc(sizeof(router_map_struct));
	memset(rt,0,sizeof(router_map_struct));
	return rt;
}

void router_list_add(router_map_struct* rt)
{
	BUS_LOG("add a rout node = %d \r\n",rt->id);
	rt->next = router_list;
	router_list = rt;
}


#endif


void packet_callback_thread(void* data)
{
	for(;;)
	{
		BUS_ASSERT(!packet_q_pend(),"packet_q_pend err");
		// if(pack_ct_cnt++ == 400)
		// {
		// 	pack_ct_cnt = 0;
		// 	printf("pt=%d\r\n", packet_q_sem.sem->OSEventCnt);
		// }
		if(packet_q_GetSize() > 0)
		{
			packet_q_t p;
//			CPU_DEF();
//			CPU_PEND();
            BUS_ASSERT(!packet_q_Delete(&p),"packet_q_Delete err");
			
//			CPU_POST();

			if(p.desId != BUS_BRAODCAST_ID)
			{
				bus_packet packet;
				//BUS_PACK_COPY_CONSTRUCT(pack,p.pack);
				{
					bus_packet_t pack = p.pack;
					packet.adapterId = p.adapterId;
					packet.srcId = p.srcId;
					packet.desId = p.desId;
					packet.sessionId = p.sessionId;
					packet.functionId = p.functionId;
					//BUS_PACK_GET_DATA_LEN(packet.data_len,pack);
					BUS_ASSERT(pack.base == NULL,"nul ptr");
					packet.data_len = pack.base->cnt;
					packet.data = (u8*)pack.base->data;
					bus_rx_callback(&packet);

					//BUS_PACK_DEL(pack);
					bus_packet_del(&pack);
                    if(pack.base != NULL)
                    {
                        printf("ref = %d\r\n", pack.base->ref);
                        printf("a%x,%x\r\n",(int)pack.base,(int)pack.base->data);
                        printf("b%x,%x\r\n",(int)p.pack.base,(int)p.pack.base->data);
                    } 
                    
                    BUS_ASSERT(pack.base!=NULL, "packet_callback_thread error");	
				}
			}
			else
			{
				bus_packet packet;
				//BUS_PACK_COPY_CONSTRUCT(pack,p.pack);
				{
					bus_packet_t pack = p.pack;
					packet.adapterId = p.adapterId;
					packet.srcId = p.srcId;
					packet.desId = p.desId;
					packet.sessionId = p.sessionId;
					packet.functionId = p.functionId;
					//BUS_PACK_GET_DATA_LEN(packet.data_len,pack);
					BUS_ASSERT(pack.base == NULL,"nul ptr");
					packet.data_len = pack.base->cnt;
					packet.data = (u8*)pack.base->data;

					bus_rx_broadcast(&packet);

					//BUS_PACK_DEL(pack);
					bus_packet_del(&pack);
				}
			}

		}

	}

}

static void rx_frame_handle(bus_frame* frame)
{
    //看下这个帧是不是应答帧 如果是的 那么直接post就完了
    if((frame->functionId == BUS_FUNC_SEND_ACK) || (frame->functionId == BUS_FUNC_RET_ACK) || (frame->functionId == BUS_FUNC_HEART_BEAT_ACK))
    {
        ack_map_post(frame->adapterId, frame->srcId, frame->sessionId,frame->functionId);
    }
	else if(frame->functionId == BUS_FUNC_HEART_BEAT_ERR_ACK)
    {
        //接收到错误直接删除
        bus_frame_id_t f_id;
        f_id.adatperId = frame->adapterId;
        f_id.functionId = BUS_FUNC_SEND;
        f_id.sessionId = frame->sessionId;
        f_id.srcId = frame->srcId;

		printf("err fid = %d,sid=%d\r\n",frame->functionId, frame->sessionId);
        //出错后生命周期已结束, 直接删除, 其余的清除操作由发送函数清理
        tx_p_map_del(&f_id);
		ack_map_post(frame->adapterId, frame->srcId, frame->sessionId,err_func_id_convert_to(frame->functionId));
    }
    else
    {
        expect_packet_insert(frame); //非应答包全部放进期望树
    }
}

#if BUS_USE_ROUTER_FUNC == 1

static void router_handler(bus_frame* frame)
{
	if(frame->desId != BUS_ROUTER_SELF_ID)
	{
		//不等于路由器自己的ID 就要开始路由
		bus_adapter* des_adp = NULL;
		bus_adapter* src_adp = NULL;
		u8 rout_contain_this_id = 0;
		if(frame->desId != 0xff)
		{
			for(router_map_struct* i = router_list ; i != NULL; i = i->next)
			{				
				if(i->id == frame->srcId)
				{
					src_adp = i->adapter;
				}else if(i->id == frame->desId)
				{
					des_adp = i->adapter;
				}
                if(des_adp != NULL && src_adp != NULL){
                    rout_contain_this_id = 1;
                    break;
                }
			}
		}
		else
		{
			for(router_map_struct* i = router_list ; i != NULL; i = i->next)
			{
				if(i->id == frame->srcId)
				{
					rout_contain_this_id = 1;
					src_adp = i->adapter;
					break;	//条件都满足就跳出
				}
			}
		}


		if(rout_contain_this_id)
		{
			//开始路由
			if(frame->desId != 0xff)
			{
				if(des_adp != NULL && src_adp != NULL && des_adp != src_adp)
				{
					//CPU_PEND();
					bus_os_pend_sem(&tx_mutex_sem, 0);
					u8  temp = des_adp->localId;
					des_adp->localId = frame->srcId;
					if(des_adp->adapter_send(des_adp,frame->desId,frame->functionId,frame->sessionId,0,frame->data_len,frame->data) != 0)
					{
						printf("adp tx err\r\n");
					}
					
					des_adp->localId = temp;

					//BUS_LOG("rt %d->%d\r\n",frame->srcId,frame->desId);
					//CPU_POST();
					bus_os_post_sem(&tx_mutex_sem);

				}
			}
			else
			{
				for(bus_adapter* i = adapterList; i != NULL; i = i->next)
				{
					if(i != src_adp)	//不是自己的通通发一遍
					{
						//CPU_PEND();
						
						bus_os_pend_sem(&tx_mutex_sem, 0);
						u8	temp = i->localId;
						i->localId = frame->srcId;
						if(i->adapter_send(i,frame->desId,frame->functionId,frame->sessionId,0,frame->data_len,frame->data))
                        {
                            printf("adp tx err\r\n");
                        }
						printf("adpid=%d\r\n",frame->srcId);
						i->localId = temp;
						//BUS_LOG("bc %d->%d\r\n",frame->srcId,frame->desId);
						//CPU_POST();
						bus_os_post_sem(&tx_mutex_sem);
					}
				}
            }
		}
		else
		{
            if(src_adp == NULL){
                //加到路由表里面去
                for(bus_adapter* i = adapterList; i != NULL; i = i->next)
                {
                    if(i->adatperId == frame->adapterId)
                    {
                        //这个adapter 加进去
                        router_map_struct* node = New_router_map_struct();
                        node->adapter = i;
                        node->id = frame->srcId;
                        router_list_add(node);
                        if(NULL != bus_new_note_callback)
                        {
                            bus_new_note_callback(node->adapter, frame->srcId);
                        }
                    }
                    
                }
            }
#ifdef BUS_CAN_BOOT_ROUTER_FUNC
            if(des_adp == NULL){
                extern bus_adapter CanAdapter;
                //这个adapter 加进去
                router_map_struct* node = New_router_map_struct();
                node->adapter = &CanAdapter;
                node->id = frame->desId;
                router_list_add(node);
                if(NULL != bus_new_note_callback)
                {
                    bus_new_note_callback(node->adapter, frame->desId);
                }
            }
#endif
		}
	}
	else
	{
        rx_frame_handle(frame);
	}
}

#endif

//队列等待线程
void rx_packet_thread(void* data)
{
	printf("rx packet rev start\r\n");
	while(1)
	{
		frame_q_Waite();
		if(frame_q_GetSize() > 0)
		{
			//从一级队列里面移动到数据结构里面去
			bus_frame frame;
			BUS_ASSERT(!frame_q_Delete(&frame), "frame_q_Delete error");

#if BUS_USE_ROUTER_FUNC == 1		//路由器直接在这里截获帧就不必在往上层报告了
			router_handler(&frame);
#else
            rx_frame_handle(&frame);
#endif
		}
	}
}

void bus_adapter_add(bus_adapter* adapter, adapter_send_type send, adapter_init_type init)
{
	adapter->adapter_init = init;
	adapter->adapter_send = send;
	adapter->next = adapterList;
	adapterList = adapter;

}

#if BUS_USE_ROUTER_FUNC == 1
u32 bus_app_init(bus_ita_call callback,bus_ita_call broadcast, BUS_NEW_NODE_CALLBACK new_node_callback)
#else
u32 bus_app_init(bus_ita_call callback,bus_ita_call broadcast)
#endif
{
	if(isinitOk)
	{
		return 0;
	}
	bus_os_create_sem(&session_id_mutex_sem, 1);
	bus_os_create_sem(&tx_mutex_sem, 1);
	bus_os_create_sem(&Bus.ack_map.ack_map_mutex, 1);
	bus_os_create_sem(&expect_packet_tree_mtx, 1);
	bus_os_create_sem(&packet_q_sem, 0);
	bus_os_create_sem(&frame_q_sem, 0);

	//所有网卡初始化
	for(bus_adapter* adapter = adapterList; adapter != NULL; adapter = adapter->next)
	{
		adapter->adapter_init(adapter, adapter->localId);	//调用网卡初始化函数
	}

	bus_rx_callback = callback;
	bus_rx_broadcast = broadcast;
#if BUS_USE_ROUTER_FUNC == 1
    bus_new_note_callback = new_node_callback;
#endif
	////创建rx和tx线程
	bus_create_thread(&TASK_RX_TCB, rx_packet_thread, TASK_RX_Stk, TASK_RX_STK_SIZE, TASK_RX_PRIO,NULL);
	bus_create_thread(&TASK_CALLBACK_TCB, packet_callback_thread, TASK_CALLBACK_Stk, TASK_CALLBACK_STK_SIZE, TASK_CALLBACK_PRIO,NULL);
	isinitOk = true;
	return 0;
}
//--------------- 下面的函数是各模块实现功能的最常用函数 ita 为 interactive 的缩写 发送之后无返回则一直阻塞

static void bus_simple_send(u32 adapterId, u32 desId,u8 functionId,u8 sessionId, u32 data_len, u8* data)
{
	bus_os_pend_sem(&tx_mutex_sem, 0);

	for(bus_adapter* adp = adapterList; adp != NULL; adp = adp->next)
	{
		if(adp->adatperId == adapterId)
		{
			int frame_count = (data_len + 5) / 6;
			for(int i = 0; i < frame_count + 1; i++)
			{
				bus_frame frame;
				frame.adapterId = adapterId;
				frame.desId = desId;
				frame.srcId = adp->localId;
				frame.functionId = functionId;
				frame.sessionId = sessionId;
				WriteU16(frame.data, i);
				
				if(i == 0)
				{
					frame.data_len = 6;
					WriteU32(frame.data + 2, data_len); // 头帧的后面2个字节放的是包的长度
				}
				else
				{
					int frame_data_len = (i == frame_count) ? ((data_len % 6) == 0 ? 6 : (data_len % 6)) : 6;
					frame.data_len = frame_data_len + 2;
					memmove(frame.data + 2, data + (i - 1) * 6, frame_data_len);//一帧八个字节
				}

				//查找网卡 发送数据
				int status = adp->adapter_send(adp, frame.desId, frame.functionId, frame.sessionId, 0, frame.data_len, frame.data);
				//printf("tx frame: frameId = %d, desId = %d, funId = %d, sesId = %d, status=%d\r\n", i, desId, functionId, sessionId,status);
                bus_os_sleep(5);
            }
			
//			printf("tx frame: frameId = %d, desId = %d, funId = %d, sesId = %d\r\n", frame_count+1, desId, functionId, sessionId);
			break; //非常重要 ！！！！！！！！！！
		}
	}
	
	bus_os_post_sem(&tx_mutex_sem);
}


//--------------- 心跳保持的发送函数 ----------------------------------------------------------------------------
u32 bus_ita_hb_send(u32 adapterId, u32 desId, u32 data_len, u8* data, u8* dataout, u32* dataoutLen)
{
	u32 code = 0;
	u8 session;
	int my_session = create_session_id();
	//检查包长度
	if(my_session == -1 || data_len > BUS_PACKAGE_MAX_LEN)
	{
		code = ERR_BUG_OCCURE;
		return code;
	}
	session = (u8)my_session;
	//tx_life:创建发送包生命周期key
	bus_frame_id_t f_id;
	f_id.adatperId = adapterId;
	f_id.functionId = BUS_FUNC_SEND;
	f_id.sessionId = session;
	f_id.srcId = desId;

	bus_os_sem sem;
	bus_os_create_sem(&sem, 0);
	ack_map_set(adapterId, desId, session, BUS_FUNC_SEND_ACK, &sem);

	bus_os_sem   sem_waite_return;      //创建等待return信号量
	bus_os_create_sem(&sem_waite_return, 0);
	expect_packet_key_t key;
	key.adapterId = adapterId;
	key.srcId = desId;
	key.sessionId = session;
	key.functionId = BUS_FUNC_RETURN;
	expect_packet_set(&key, &sem_waite_return);
	
	//tx_life:发送包加入生命周期管理
	int v = -1;
	tx_p_map_insert(&f_id,&v);

	bool find_ack = false;
	for(u8 x = 0; x < 10; x++)
	{
		bus_simple_send(adapterId, desId, BUS_FUNC_SEND, session, data_len, data);
		if(bus_os_pend_sem(&sem, BUS_WAITE_ACK_TIME) == 0)
		{
			// 收到了ack 直接跳过去
			find_ack = true;
			break;
		}
		else
		{
			BUS_LOG("waite ack timeout desId = %d , sessionId = %d ,func=%d,adp=%d \r\n", desId, session, BUS_FUNC_SEND_ACK, adapterId);
		}
	}
	//发送等待应答信号量也要删除
	ack_map_erase(adapterId, desId, session, BUS_FUNC_SEND_ACK);    //不管怎样 都要删掉
	bus_os_del_sem(&sem);       //信号量也千万不要忘记删掉！！！
	if(!find_ack)
	{
		expect_packet_del(&key, NULL, dataout, dataoutLen);	//等待包删除
		bus_os_del_sem(&sem_waite_return);		//等待回复信号量删除
		BUS_LOG("send but no ack\r\n");

		//未收到ack信号, 生命周期结束
		tx_p_map_del(&f_id);
		destroy_session_id(f_id.sessionId);
		return ERR_BUS_ITA_NO_ACK;
	}

	bus_os_sem heart_beat_sem;
	bus_os_create_sem(&heart_beat_sem, 0);
	ack_map_set(adapterId, desId, session, BUS_FUNC_HEART_BEAT_ACK, &heart_beat_sem); //等待回复超时用心跳来维持

	while(true)
	{
		code = bus_os_pend_sem(&sem_waite_return,BUS_WAITE_RETURN_TIME);	//查看下返回包有没有到达
		if(code != 0)
		{
			//发送心跳包并等待返回
			bool no_heart_beat = true;
			for(int x = 0; x<3; x++)
			{
				bus_simple_send(adapterId, desId, BUS_FUNC_HEART_BEAT,session, 0, NULL);
				code = bus_os_pend_sem(&heart_beat_sem, BUS_WAITE_ACK_TIME);
				if(code == 0)
				{
					//生存周期已结束, 不需要再维持心跳了, 直接报错清理, 生存周期包在接收时被删除了
					//可能是接收到错误信息
					//生命周期参数被删除
					//TODO::这里最好重新定义错误类型
					if(tx_p_map_serach(&f_id) == NULL)
					{
						no_heart_beat = true;
						printf("sd hb compare=%d not alive\r\n,",f_id.compare);
					}else
					{
						no_heart_beat = false;
					}
					break;
				}
			}
			if(no_heart_beat)
			{
				bus_os_del_sem(&sem_waite_return);
				bus_os_del_sem(&heart_beat_sem);
				ack_map_erase(adapterId, desId, session, BUS_FUNC_HEART_BEAT_ACK);
				expect_packet_del(&key, NULL, dataout, dataoutLen);	//等待包删除
				//心跳信号ack未收到, 生命周期结束
				destroy_session_id(f_id.sessionId);
				tx_p_map_del(&f_id);
				return  ERR_HEART_BEAT_TIMOUT;
			}
		}
		else
		{
			//等到了回复包  return ack 已经给过了
			bus_os_del_sem(&sem_waite_return);
			bus_os_del_sem(&heart_beat_sem);
			ack_map_erase(adapterId, desId, session, BUS_FUNC_HEART_BEAT_ACK);
			expect_packet_del(&key, NULL, dataout, dataoutLen);	//等待包删除
			destroy_session_id(f_id.sessionId);
			//正常发送成功, 生命周期结束
			tx_p_map_del(&f_id);
			return 0;
		}
	}
}



//如果ID不是0xff 那么就是单播 否则就是广播  广播就没有错误重发的机制了 请使用的时候判断场合 进行使用
u32 bus_ita_broadcast(u32 adapterId, u32 data_len, u8* data)
{
	int session = create_session_id();
	if(session == -1)
	{
		return ERR_BUG_OCCURE;
	}
	bus_simple_send(adapterId, BUS_BRAODCAST_ID, BUS_FUNC_SEND, (u8)session, data_len, data);
	return 0;
}

u32 bus_ita_return(u32 adapterId, u32 desId, u8 ita_session, u32 data_len, u8* data)
{
	bus_frame_id_t frame_id;
	frame_id.adatperId = adapterId;
	frame_id.srcId = desId;
	frame_id.sessionId = ita_session;
	frame_id.functionId = BUS_FUNC_SEND;

	if(data_len > BUS_PACKAGE_MAX_LEN)
	{
		rx_p_map_del(&frame_id, true);//？？？
		return ERR_BUG_OCCURE;
	}
	bus_os_sem sem;
	bus_os_create_sem(&sem, 0);
	ack_map_set(adapterId, desId, ita_session, BUS_FUNC_RET_ACK, &sem);//设置应答map

	for(u8 x = 0; x < 10; x++)
	{
		bus_simple_send(adapterId, desId, BUS_FUNC_RETURN, ita_session, data_len, data);//通过总线向上位机发送数据，每一帧8字节
		if(bus_os_pend_sem(&sem, BUS_WAITE_ACK_TIME) == 0)//如果err=0，是等到了信号量
		{
			bus_os_del_sem(&sem);//删除信号量
			goto FOUND; // 收到了ack 直接跳过去
		}
		else
		{
			BUS_LOG("re rt\r\n");
		}
	}
	ack_map_erase(adapterId, desId, ita_session, BUS_FUNC_RET_ACK);
	bus_os_del_sem(&sem);
	
	//rx_life:返回心跳超时, 接收的数据生命周期进入接收完毕阶段
	rx_p_map_del(&frame_id, false);
	BUS_LOG("ERR_BUS_RETURN_NO_ACK\r\n");
	return ERR_BUS_RETURN_NO_ACK;
FOUND:
	ack_map_erase(adapterId, desId, ita_session, BUS_FUNC_RET_ACK);
	//rx_life:返回完毕, 接收的数据生命周期进入接收完毕阶段
	rx_p_map_del(&frame_id, false);
	return 0;
}

void bus_ita_return_send_err(u32 adapterId, u32 desId, u8 ita_session)
{
    bus_frame_id_t frame_id;
	frame_id.adatperId = adapterId;
	frame_id.srcId = desId;
	frame_id.sessionId = ita_session;
	frame_id.functionId = BUS_FUNC_SEND;
	//报错, 生命周期强行进入接收完成阶段
	rx_p_map_del(&frame_id, true);
}

// static volatile int rev_time = 0;

//--------------- 下面是给驱动调用的函数         -----------------------------------------------------------
void bus_for_driver_call(u32 adapterId, u32 srcId, u32 desId, u32 functionId, u32 sessionId, u32 data_len, u8* data)
{
	if(!isinitOk)
	{
		return ;
	}

	bus_frame frame;
	frame.adapterId = adapterId;
	frame.srcId = srcId;
	frame.desId = desId;
	frame.functionId = functionId;
	frame.sessionId = sessionId;
	frame.data_len = data_len;
    BUS_ASSERT(data_len>8, "frame len error");
	memmove(frame.data, data, data_len);

	// if((rev_time++)==400)
	// {
	// 	rev_time = 0;
	// 	printf("rv:%d\r\n",frame_q_sem.sem->OSEventCnt);
	// }

	BUS_ASSERT(!frame_q_Enter(&frame),"frame_q_Enter error");
}

// 对不支持的指令码进行报错
void bus_ita_return_unkonw_cmd(bus_packet* packet)
{
	if(packet != NULL)
	{
		bus_ita_return_send_err(packet->adapterId, packet->srcId, packet->sessionId);
	}
}

void my_bus_simple_send(u32 adapterId, u32 srcId, u32 desId, u32 functionId, u32 sessionId, u32 data_len, u8* data)
{
	bus_simple_send(adapterId, desId, functionId,sessionId, data_len, data);
}

bool bus_is_init_ok(void)
{
	return isinitOk;
}




