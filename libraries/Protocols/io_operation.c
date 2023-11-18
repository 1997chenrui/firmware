

// 此文件功能：勾手传感器状态读取，上位机通过下发指令读取传感器当前的状态



#include "io_operation.h"
#include "Protocols.h"
#include "data_struct.h"
#include "stm32f4xx_hal.h"
 

#ifdef EXTENDED_IO_EN
#include "LS165_HC595_Cfg.h"
#endif

#define IO_THREAD_STACK_SIZE 	256
#define IO_THREAD_PRIO			15

static OnSimpleIoCtrlCallBack onSimpleIoCtrlCallBack = NULL;

typedef struct
{
	union
	{
		struct
		{
			u16 port : 8;
			u16 pin : 8;
		}data;
		u16 port_pin;
	}PP;
	union
	{
		struct
		{
			u8 cmd : 4;
			u8 arg : 1;
		}data;
		u8 cmd_arg;
	}CA;
}IOSend_Struct;

typedef union
{
	struct
	{
		u32 port : 8;
		u32 pin : 16;
	}data;
	u32 read_data;
}IORT_Struct;
void Io_Thread(void *arg);

static struct
{
    bus_os_thread            tcb;
    bus_os_sem               sem;
    u32                    	 stack[IO_THREAD_STACK_SIZE];
}IoThread_Cfg;

SeqQueue_Struct	io_queqe;

						
static bool io_queue_pend()            
{                                        
	bus_os_pend_sem(&IoThread_Cfg.sem,0);    
	return true;                        
}                                        
static bool io_queue_post()         
{                                        
	bus_os_post_sem(&IoThread_Cfg.sem);
	return true;                        
}

void Io_OperationInit(void)
{
	bus_create_thread(&IoThread_Cfg.tcb, Io_Thread, IoThread_Cfg.stack, IO_THREAD_STACK_SIZE, IO_THREAD_PRIO, NULL);
	bus_os_create_sem(&IoThread_Cfg.sem, 0);
	SeqQueue_Init(&io_queqe, sizeof(Io_Queue_Ele_t), &io_queue_pend, &io_queue_post);
}


void Io_Queue_Enter(Io_Queue_Ele_t *ele)
{
    SeqQueue_Enter(&io_queqe,ele);
}

//
void setSimpleIoCtrlEvent(OnSimpleIoCtrlCallBack callback)
{
	onSimpleIoCtrlCallBack = callback;
}

//预留 2019/7/24
void clearSimpleIoCtrlEvent()
{
	onSimpleIoCtrlCallBack = NULL;
}

static u16 PinNumToGPIO_Pin(u16 pin)
{
    u16 temp_pin = 0;
	switch(pin)
	{
		case 0: temp_pin= GPIO_PIN_0;break;
		case 1: temp_pin= GPIO_PIN_1;break;
		case 2: temp_pin= GPIO_PIN_2;break;
		case 3: temp_pin= GPIO_PIN_3;break;
		case 4: temp_pin= GPIO_PIN_4;break;
		case 5: temp_pin= GPIO_PIN_5;break;
		case 6: temp_pin= GPIO_PIN_6;break;
		case 7: temp_pin= GPIO_PIN_7;break;
		case 8: temp_pin= GPIO_PIN_8;break;
		case 9: temp_pin= GPIO_PIN_9;break;
		case 10: temp_pin = GPIO_PIN_10;break;
		case 11: temp_pin = GPIO_PIN_11;break;
		case 12: temp_pin = GPIO_PIN_12;break;
		case 13: temp_pin = GPIO_PIN_13;break;
		case 14: temp_pin = GPIO_PIN_14;break;
		case 15: temp_pin = GPIO_PIN_15;break;
		default:printf("GPIO PIN Parameter error\r\n");break;
	}
	return temp_pin;
}


void IO_cfg(u8 port , u16 pin , bool isOut)
{
    GPIO_InitTypeDef io;
    memset(&io,0,sizeof(io));
    if(!isOut)
    {
        io.Pin = PinNumToGPIO_Pin(pin);
        io.Mode = GPIO_MODE_INPUT;
        io.Pull = GPIO_PULLUP;
        io.Speed = GPIO_SPEED_FAST;
    }
    else
    {
        io.Pin = PinNumToGPIO_Pin(pin);
        io.Mode = GPIO_MODE_OUTPUT_PP;
        io.Pull = GPIO_NOPULL;
        io.Speed = GPIO_SPEED_FAST;
    }

    switch(port)
    {
        case 0:HAL_GPIO_Init(GPIOA, &io);break;
        case 1:HAL_GPIO_Init(GPIOB, &io);break;
        case 2:HAL_GPIO_Init(GPIOC, &io);break;
        case 3:HAL_GPIO_Init(GPIOD, &io);break;
        case 4:HAL_GPIO_Init(GPIOE, &io);break;
        case 5:HAL_GPIO_Init(GPIOF, &io);break;
        case 6:HAL_GPIO_Init(GPIOG, &io);break;
    }
    
    
}



static void io_write(u8 port,u16 pin,GPIO_PinState val)
{
	switch(port)
	{
		case 0:IO_cfg (port,pin,true); HAL_GPIO_WritePin(GPIOA,PinNumToGPIO_Pin(pin),val);break;
		case 1:IO_cfg (port,pin,true); HAL_GPIO_WritePin(GPIOB,PinNumToGPIO_Pin(pin),val);break;
		case 2:IO_cfg (port,pin,true); HAL_GPIO_WritePin(GPIOC,PinNumToGPIO_Pin(pin),val);break;
		case 3:IO_cfg (port,pin,true); HAL_GPIO_WritePin(GPIOD,PinNumToGPIO_Pin(pin),val);break;
		case 4:IO_cfg (port,pin,true); HAL_GPIO_WritePin(GPIOE,PinNumToGPIO_Pin(pin),val);break;
		case 5:IO_cfg (port,pin,true); HAL_GPIO_WritePin(GPIOF,PinNumToGPIO_Pin(pin),val);break;
		case 6:IO_cfg (port,pin,true); HAL_GPIO_WritePin(GPIOG,PinNumToGPIO_Pin(pin),val);break;
#ifdef EXTENDED_IO_EN
		case 7:
		case 8:Extended_IO_WRITE(port,pin,val);break;
#endif
		default: 
            printf("io write error port=%d\r\n",port);break;
	}
}
static u16 io_read(u8 port)
{
	u16 temp_value = 0;
	switch(port)
	{
		case 0:temp_value = GPIOA->IDR;break;
		case 1:temp_value = GPIOB->IDR;break;
		case 2:temp_value = GPIOC->IDR;break;
		case 3:temp_value = GPIOD->IDR;break;
		case 4:temp_value = GPIOE->IDR;break;
		case 5:temp_value = GPIOF->IDR;break;
		case 6:temp_value = GPIOG->IDR;break;
#ifdef EXTENDED_IO_EN
		case 7:
		case 8:temp_value = Extended_IO_READ(port); break;
#endif
		default:
            printf("io_read parameter error port=%d\r\n",port);break;
	}
	return temp_value;
}
static void io_toggle(u8 port,u16 pin)
{
	switch(port)
	{
		case 0:HAL_GPIO_TogglePin(GPIOA,PinNumToGPIO_Pin(pin));break;
		case 1:HAL_GPIO_TogglePin(GPIOB,PinNumToGPIO_Pin(pin));break;
		case 2:HAL_GPIO_TogglePin(GPIOC,PinNumToGPIO_Pin(pin));break;
		case 3:HAL_GPIO_TogglePin(GPIOD,PinNumToGPIO_Pin(pin));break;
		case 4:HAL_GPIO_TogglePin(GPIOE,PinNumToGPIO_Pin(pin));break;
		case 5:HAL_GPIO_TogglePin(GPIOF,PinNumToGPIO_Pin(pin));break;
		case 6:HAL_GPIO_TogglePin(GPIOG,PinNumToGPIO_Pin(pin));break;
#ifdef EXTENDED_IO_EN
		case 7:
		case 8:Extended_IO_TOGGLE(port,pin);break;
#endif
		default:
            printf("io_toggle parameter error port=%d\r\n",port);break;
	}
}


// 基于168MHz的微妙级延时
static void IO_delay(unsigned int us)
{
	unsigned int i;
	unsigned char k = 0;
	
	for(i = 0; i < us; i++)
	{
		for(k = 0; k < 10; k++)
		{
		}
	}
}


void Io_Thread(void *arg)
{
	//printf("%s %d \r\n",__FUNCTION__,__LINE__);
	printf("Io_Thread Start. Prio = %d\r\n",IO_THREAD_PRIO);
	BytesBufferNode* Io_node = NULL;
	
	while(1)
	{
		SeqQueue_Waite(&io_queqe);
        Io_Queue_Ele_t ele;
        SeqQueue_Delete(&io_queqe,&ele);
		u32 rt_buff_len = 0;
		u8 *rt_buff = NULL;
		
		IoOperation_Send *ctrl  = (IoOperation_Send *)ele.send;
		IoOperation_Return *rt = New_IoOperation_Return(); 

		
		for(u8 k = 0;k<ctrl->m1_IoArgs_Length;k++)
		{
			for(u8 i = 0; i < ctrl->m1_IoArgs[k].m1_Controls_Length; i++)
			{
				IOSend_Struct IO_S;
				
				IO_S.PP.port_pin = ctrl->m1_IoArgs[k].m1_Controls[i].m1_port_pin;
				IO_S.CA.cmd_arg = ctrl->m1_IoArgs[k].m1_Controls[i].m2_cmd_arg;
				
			   // 0 is read , 1 is write , 2 is toggle
				switch(IO_S.CA.data.cmd)
				{
					case 0:
					{
						IORT_Struct io_r;
						io_r.data.port = IO_S.PP.data.port;
						for(u8 j = 0; j < 3; j++)
						{
							io_r.data.pin = io_read(IO_S.PP.data.port);
							IO_delay(5); // 读取传感器状态时需要延时消抖, icode changed
						}
						rt->m2_value = io_r.read_data;
						break;
					}
					case 1:
					{
						 io_write(IO_S.PP.data.port,IO_S.PP.data.pin,(GPIO_PinState)IO_S.CA.data.arg);
						 break;
					}
					 case 2:
					 {
						 io_toggle(IO_S.PP.data.port,IO_S.PP.data.pin);
						 break;
					 }
					 default:printf("O_S.CA.data.cm error\r\n");break;
				}
			}
            Delete_IoOperation_Send_m1_IoArgs_m1_Controls(ctrl->m1_IoArgs[k].m1_Controls);
			bus_os_sleep(ctrl->m1_IoArgs[k].m2_delay);
		}
		rt->m1_errCode = 0;
		
		Io_node = rt->ToBytes(rt);
		rt_buff_len = BytesBufferNode_Destroy(Io_node , &rt_buff);

		bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,rt_buff_len,rt_buff);
		free(rt_buff);
        
		Delete_IoOperation_Send_m1_IoArgs(ctrl->m1_IoArgs);
		Delete_IoOperation_Send(ctrl);
		Delete_IoOperation_Return(rt);
	 }		 
}

bool is_GPIOA_2=0;
u32 Io_Sample(u16 port_pin_arg)
{
	printf("m1_port_pin_arg = %d\r\n",port_pin_arg);
	union
	{
		struct
		{
			u16 port : 3;
			u16 pin  : 4;
			u16 fun  : 2;	//0:read 1:write 2 tri
			u16 arg  : 1;
		}data;
		u16 data_arg;
	}DA;
	
	union
	{
		struct
		{
			u32 value : 1;
			u32 err_code:31;
		}data;
		u32 data_err;
	}DE;

	DE.data_err = 0;
	DA.data_arg = port_pin_arg;
	//PA2震荡电机
	if( DA.data.port==0 &&  DA.data.pin==2 )
	{
		is_GPIOA_2=1;
	}
	printf("port=%d,pin=%d,fun=%d,arg=%d\r\n",DA.data.port,DA.data.pin,DA.data.fun,DA.data.arg);
	switch(DA.data.fun)
	{
		case 0:
		{
            IO_cfg(DA.data.port,DA.data.pin,false);

//原代码
			switch(DA.data.port)
			{
				case 0:DE.data.value=HAL_GPIO_ReadPin(GPIOA,PinNumToGPIO_Pin(DA.data.pin));break;
				case 1:DE.data.value=HAL_GPIO_ReadPin(GPIOB,PinNumToGPIO_Pin(DA.data.pin));break;
				case 2:DE.data.value=HAL_GPIO_ReadPin(GPIOC,PinNumToGPIO_Pin(DA.data.pin));break;
				case 3:DE.data.value=HAL_GPIO_ReadPin(GPIOD,PinNumToGPIO_Pin(DA.data.pin));break;
				case 4:DE.data.value=HAL_GPIO_ReadPin(GPIOE,PinNumToGPIO_Pin(DA.data.pin));break;
				case 5:DE.data.value=HAL_GPIO_ReadPin(GPIOF,PinNumToGPIO_Pin(DA.data.pin));break;
				case 6:DE.data.value=HAL_GPIO_ReadPin(GPIOG,PinNumToGPIO_Pin(DA.data.pin));break;
			}
			 
			//int temp_data  = ((DE.data.value >> DA.data.pin) & 1);
//			if(DA.data.pin==10)printf("PB10\r\n");
			printf("value=%d\r\n",DE.data.value);
			DE.data.err_code = 0;
			break;
		}
		case 1:
		{
			 io_write(DA.data.port,DA.data.pin,(GPIO_PinState)DA.data.arg);
			 DE.data_err = 0;
			 break;
		}
		case 2:
		{
			 io_toggle(DA.data.port,DA.data.pin);
			 DE.data_err = 0;
			 break;
		}
	}
	if(onSimpleIoCtrlCallBack != NULL)
	{
		onSimpleIoCtrlCallBack(DA.data.port,DA.data.pin,DA.data.fun,DA.data.arg);
	}
	printf("data_err=%d\r\n",DE.data_err);
	return DE.data_err;
}

