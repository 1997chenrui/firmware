#include "cmdExcute.h"
#include "ErrCode.h"
#include "motor.h"
#include "motor_thread.h"
#ifdef TURNTABLES_MOTOR_EN
#include "turntables_motor.h"
#endif
#ifdef ENCODETURNTABLE_EN
#include "encode_turntable.h"
#endif

#include "stm32f4xx_hal.h"
#include <math.h>

//------------- 函数声明 ------------------
static void CmdHandle_SeqMotorultiMove(Motor_Queue_Ele_t *ele);


static void ce_assert(bool exp,int line)
{
    if(exp)
    {
        while(1)
        {
            printf("ce err %d \r\n",line);
        }
    }
}

#define CE_ASSERT( exp ) ce_assert((exp),__LINE__)



SeqQueue_Struct             queue[MOTOR_THEARD_CNT];        //队列成员应该是命令码和send对象


static int readIo(void * port,u16 pin)
{
    return HAL_GPIO_ReadPin(port,pin);
}

static GPIO_TypeDef * getPort(int idx)
{
    switch(idx)
    {
        case 0: __HAL_RCC_GPIOA_CLK_ENABLE(); return GPIOA;
        case 1: __HAL_RCC_GPIOB_CLK_ENABLE(); return GPIOB;
        case 2: __HAL_RCC_GPIOC_CLK_ENABLE(); return GPIOC;
        case 3: __HAL_RCC_GPIOD_CLK_ENABLE(); return GPIOD;
        case 4: __HAL_RCC_GPIOE_CLK_ENABLE(); return GPIOE;
        case 5: __HAL_RCC_GPIOF_CLK_ENABLE(); return GPIOF;
        case 6: __HAL_RCC_GPIOG_CLK_ENABLE(); return GPIOG;
        case 7: __HAL_RCC_GPIOH_CLK_ENABLE(); return GPIOH;

        default :
        {
            printf("error Io = %d\r\n",idx);
            return GPIOI;
        }
    }
}

static u16 getPin(int idx)
{
    switch(idx)
    {
        case 0: return GPIO_PIN_0 ;
        case 1: return GPIO_PIN_1 ;
        case 2: return GPIO_PIN_2 ;
        case 3: return GPIO_PIN_3 ;
        case 4: return GPIO_PIN_4 ;
        case 5: return GPIO_PIN_5 ;
        case 6: return GPIO_PIN_6 ;
        case 7: return GPIO_PIN_7 ;
        case 8: return GPIO_PIN_8 ;
        case 9: return GPIO_PIN_9 ;
        case 10: return GPIO_PIN_10 ;
        case 11: return GPIO_PIN_11 ;
        case 12: return GPIO_PIN_12 ;
        case 13: return GPIO_PIN_13 ;
        case 14: return GPIO_PIN_14 ;
        case 15: return GPIO_PIN_15 ;
        default: 
             printf("err pin=%d\r\n",idx);
             return 0 ;
    }
}


static int thread_idx = 0;
void  Motor_Thread(void *arg)
{
    printf("Motor_Thread Start.thread_idx = %d \r\n",thread_idx);
    BytesBufferNode* temp_node = NULL;
    int td_idx = thread_idx++;

#if 0    
    if(td_idx == 1)
    {
        while(1)
        {
            motor_start_listen_io(&motors[0],MOTOR_LSN_IO_STATE_RISING,getPort(3),getPin(1),readIo);
            motor_move(&motors[0],100000,4000,40000);
            bus_os_sleep(1000);
        }
    }

#endif
    
    while(1)
    {
        SeqQueue_Waite(&queue[td_idx]);
        Motor_Queue_Ele_t ele;
        SeqQueue_Delete(&queue[td_idx],&ele);
        switch(ele.cmd)
        {
            case CMD_MotorReset:
            {
                printf("thread %d reset \r\n",td_idx);
                MotorReset_Send *reset  = (MotorReset_Send *)ele.send;
                MotorReset_Return * rt = New_MotorReset_Return();
                rt->m1_errCode = motor_reset(&motors[td_idx],reset->m3_speed,reset->m2_acce);
                temp_node =    rt->ToBytes(rt);
                u8 *data = NULL;
                int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_MotorReset_Send(reset);
                Delete_MotorReset_Return(rt);
                break;
            }
            
            case CMD_MotorMove :
            {
				//printf("CMD_MotorMove\r\n");
                MotorMove_Send *mv = (MotorMove_Send *)ele.send;
                MotorMove_Return *rt = New_MotorMove_Return();

                if(mv->m1_idx == td_idx)
                {
// 					printf("m4_distance=%d\r\n",mv->m4_distance);
// 					printf("m3_speed=%d\r\n",mv->m3_speed);
// 					printf("m2_acce=%d\r\n",mv->m2_acce);
// 					printf("m5_isRelative=%d\r\n",mv->m5_isRelative);
                    if(mv->m5_isRelative)
                    {
                        rt->m1_errCode =  motor_move(&motors[mv->m1_idx],mv->m4_distance,mv->m3_speed,mv->m2_acce);
                    }
                    else
                    {
                        rt->m1_errCode =  motor_move_absolute(&motors[mv->m1_idx],mv->m4_distance,mv->m3_speed,mv->m2_acce,mv->m6_isBackZero);
                    }
                    temp_node = rt->ToBytes(rt);
                    u8 *data = NULL;
                    int len = BytesBufferNode_Destroy(temp_node,&data);
                    int rt_start = OSTimeGet();
                    CE_ASSERT(len != 4);
                    if( *((u32*)data)  != 0  )
                    {
                        printf("mv%x\r\n",(*((u32*)data)));
                    }
                    bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                    if(rt->m1_errCode != 0 )
                    {
                        printf("err %x\r\n",rt->m1_errCode);
                    }
                    free(data);
                    Delete_MotorMove_Send(mv);
                    Delete_MotorMove_Return(rt);
                }
                else
                {
                    //TODO
                    
                }
                //printf("mv end %d \r\n",OSTimeGet() - start);     

//                pre_end = OSTimeGet();
                break;
            }

            case CMD_MotorMultiMove :
            {
				//printf("CMD_MotorMultiMove\r\n");
                MotorMultiMove_Send *mmv = (MotorMultiMove_Send *)ele.send;
                MotorMultiMove_Return *rt = New_MotorMultiMove_Return();
    
                if(mmv->m1_MoveArgs_Length > 0 )
                {
                    motor_multi_move_args_t * margs = malloc(sizeof(motor_multi_move_args_t)*mmv->m1_MoveArgs_Length);
                    CE_ASSERT(margs == NULL);
                    memset(margs,0,sizeof(motor_multi_move_args_t)*mmv->m1_MoveArgs_Length);
                    
                    for(int i = 0;i<mmv->m1_MoveArgs_Length;i++)
                    {
                        margs[i].motor = &motors[mmv->m1_MoveArgs[i].m1_idx];
                        margs[i].acce=  mmv->m1_MoveArgs[i].m2_acce;
                        margs[i].speed= mmv->m1_MoveArgs[i].m3_speed;
                        margs[i].steps= mmv->m1_MoveArgs[i].m4_distance;
                        margs[i].is_relative = mmv->m1_MoveArgs[i].m5_isRelative;
                    }
    
                    motor_multi_move_absolute(mmv->m1_MoveArgs_Length,margs,mmv->m2_isBackZero);
    
                    for(int i = 0;i<mmv->m1_MoveArgs_Length;i++)
                    {
                        if(margs[i].errCode)
                        {
                            rt->m1_errCode = margs[i].errCode;
                            break;
                        }
                    }
                    free(margs);        //释放参数内存
    
                }
                else
                {
                    rt->m1_errCode = ERR_MOTOR_ARGS_ERROR;
                }
                BytesBufferNode * node =  rt->ToBytes(rt);
                u8 *data = NULL;
                int len = BytesBufferNode_Destroy(node,&data);
                CE_ASSERT(len != 4);
                if( *((u32*)data)  != 0  )
                {
                    printf("ml%x\r\n",(*((u32*)data)));
                }
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
    
                free(data);
                Delete_MotorMultiMove_Send_m1_MoveArgs(mmv->m1_MoveArgs);
                Delete_MotorMultiMove_Send(mmv );
                Delete_MotorMultiMove_Return(rt);

                break;
            }

            case CMD_SeqMotorMultiMove :
            {
				printf("CMD_SeqMotorMultiMove\r\n");
                CmdHandle_SeqMotorultiMove(&ele);
                break;
            }
            
            case CMD_MotorMoveSeq :
            {
				printf("CMD_MotorMoveSeq\r\n");
                MotorMoveSeq_Send   *send = (MotorMoveSeq_Send *)ele.send;
                MotorMoveSeq_Return *rt = New_MotorMoveSeq_Return();
                for(int i =0;i<send->m1_MoveArgs_Length;i++)
                {
                    
                    if(send->m1_MoveArgs[i].m5_isRelative)
                    {
                        rt->m1_errCode =  motor_move(&motors[send->m1_MoveArgs[i].m1_idx],send->m1_MoveArgs[i].m4_distance,send->m1_MoveArgs[i].m3_speed,send->m1_MoveArgs[i].m2_acce);
                    }
                    else
                    {
                        rt->m1_errCode =  motor_move_absolute(&motors[send->m1_MoveArgs[i].m1_idx],send->m1_MoveArgs[i].m4_distance,send->m1_MoveArgs[i].m3_speed,send->m1_MoveArgs[i].m2_acce,false);
                    }
                    if(rt->m1_errCode != 0)
                        break;
                    bus_os_sleep(send->m1_MoveArgs[i].m6_delayms);
                }
                
                temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
                int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_MotorMoveSeq_Send_m1_MoveArgs(send->m1_MoveArgs);
                Delete_MotorMoveSeq_Send(send);
                Delete_MotorMoveSeq_Return(rt);
                break;
            }

            case CMD_MotorRotate :
            {
				printf("CMD_MotorRotate\r\n");
                MotorRotate_Send    *send   =  (MotorRotate_Send *)ele.send;
                MotorRotate_Return  *rt     = New_MotorRotate_Return();
                
                if(send->m5_isRelative)
                {
                    rt->m1_errCode=motor_rotate(&motors[send->m1_idx],send->m4_distance,send->m3_speed,send->m2_acce);
                }
                else
                {
                    rt->m1_errCode=motor_rotate_absolute(&motors[send->m1_idx],send->m4_distance,send->m3_speed,send->m2_acce);
                }
//				TMotor_Move(2,665,send->m4_distance,send->m4_distance,send->m2_acce);
				
                temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
                int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_MotorRotate_Send(send);
                Delete_MotorRotate_Return(rt);
                break;
            }
#ifdef TURNTABLES_MOTOR_EN
            case CMD_TurnTableMove:
			{
				TurnTable_Move_Send *mv = (TurnTable_Move_Send *)ele.send;
				TurnTable_Move_Return  *rt     = New_TurnTable_Move_Return();
// 				printf("mv->m1_cali_location  = %d \r\n",mv->m1_cali_location);
// 				printf("mv->m2_idx = %d \r\n",			 mv->m2_idx);
// 				printf("mv->m3_steps = %d \r\n",		 mv->m3_steps);
// 				printf("mv->m4_speed = %d \r\n",		 mv->m4_speed);
// 				printf("mv->m5_acce = %d \r\n",			 mv->m5_acce);
				rt->m1_errCode   = TMotor_Move(mv->m2_idx,mv->m1_cali_location,mv->m3_steps,mv->m4_speed,mv->m5_acce);
				rt->m2_filter_id = TMotor_GetFilterId();
				rt->m3_ad_data	 = TMotor_GetAdData();
// 				printf("rt->m1_errCode   = %d \r\n", rt->m1_errCode);
// 				printf("rt->m2_filter_id = %d \r\n", rt->m2_filter_id);
// 				printf("rt->m3_ad_data   = %d \r\n", rt->m3_ad_data);
				
				temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_TurnTable_Move_Send(mv);
                Delete_TurnTable_Move_Return(rt);
				break;
			}
#endif
#ifdef ENCODETURNTABLE_EN
            case CMD_EncodeTurnTableFindZero:
			{
				printf("CMD_EncodeTurnTableFindZero\r\n");
				EncodeTurnTableFindZero_Send *mv = (EncodeTurnTableFindZero_Send *)ele.send;
				EncodeTurnTableFindZero_Return  *rt     = New_EncodeTurnTableFindZero_Return();
//				printf("mv->m2_idx = %d \r\n",			 mv->m1_idx);
				printf("fz:mv->m1_cali_location  = %d \r\n",mv->m2_cali_location);
//				printf("mv->m4_speed = %d \r\n",		 mv->m3_speed);
//				printf("mv->m5_acce = %d \r\n",			 mv->m4_acce);
				rt->m1_errCode   = encode_turntable_reset(&ETruntable[mv->m1_idx],mv->m2_cali_location,mv->m3_speed,mv->m4_acce);

//				printf("rt->m1_errCode   = %d \r\n", rt->m1_errCode);
				
				temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_EncodeTurnTableFindZero_Send(mv);
                Delete_EncodeTurnTableFindZero_Return(rt);
				break;
			}
			case CMD_EncodeTurnTableRotateStart:
			{
				printf("CMD_EncodeTurnTableRotateStart\r\n");
				EncodeTurnTableRotateStart_Send *mv = (EncodeTurnTableRotateStart_Send *)ele.send;
				EncodeTurnTableRotateStart_Return  *rt     = New_EncodeTurnTableRotateStart_Return();
//				printf("mv->m2_idx = %d \r\n",			 mv->m1_idx);
				printf("R:mv->m1_cali_location  = %d \r\n",mv->m2_cali_location);
//				printf("mv->m4_speed = %d \r\n",		 mv->m3_speed);
//				printf("mv->m5_acce = %d \r\n",			 mv->m4_acce);
				rt->m1_errCode   = encode_turntable_rotate_start(&ETruntable[mv->m1_idx],mv->m2_cali_location,mv->m3_speed,mv->m4_acce);

//				printf("rt->m1_errCode   = %d \r\n", rt->m1_errCode);
				
				temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_EncodeTurnTableRotateStart_Send(mv);
                Delete_EncodeTurnTableRotateStart_Return(rt);
				break;
			}
			case CMD_EncodeTurnTableSignalSend:
			{
				printf("CMD_EncodeTurnTableSignalSend\r\n");
				EncodeTurnTableSignalSend_Send *mv = (EncodeTurnTableSignalSend_Send *)ele.send;
				EncodeTurnTableSignalSend_Return  *rt     = New_EncodeTurnTableSignalSend_Return();
//				printf("mv->m1_idx = %d \r\n",			 mv->m1_idx);
//				printf("mv->m2_dest_id  = %d \r\n",mv->m2_dest_id);
//				printf("mv->m3_signal_type = %d \r\n",		 mv->m3_signal_type);
				rt->m1_errCode   = encode_turntable_send_signal(&ETruntable[mv->m1_idx],mv->m2_dest_id,mv->m3_signal_type);

//				printf("rt->m1_errCode   = %d \r\n", rt->m1_errCode);
				
				temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_EncodeTurnTableSignalSend_Send(mv);
                Delete_EncodeTurnTableSignalSend_Return(rt);
				break;
			}
			case CMD_EncodeTurnTableStateGet:
			{
				printf("CMD_EncodeTurnTableStateGet\r\n");
				EncodeTurnTableStateGet_Send *get = (EncodeTurnTableStateGet_Send *)ele.send;
				EncodeTurnTableStateGet_Return  *rt     = New_EncodeTurnTableStateGet_Return();
				printf("mv->m1_idx = %d \r\n",			 get->idx);
				
				
				encode_turntable_state_get(&ETruntable[get->idx],
											&rt->m3_current_id,
											&rt->m2_is_busy,
											&rt->m1_isZeroTriggered);
				
				
				temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
				int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_EncodeTurnTableStateGet_Send(get);
                Delete_EncodeTurnTableStateGet_Return(rt);
				break;
			}
#endif
            case CMD_MotorRotateStart :
            {
// 				printf("CMD_MotorRotateStart\r\n");
                MotorRotateStart_Send *send = (MotorRotateStart_Send *)ele.send;
                MotorRotateStart_Return *rt = New_MotorRotateStart_Return();
                rt->m1_errCode = motor_rotate_start(&motors[send->m1_idx],send->m4_isForward,send->m3_speed,send->m2_acce);
                temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
                int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_MotorRotateStart_Send(send);
                Delete_MotorRotateStart_Return(rt);
                
                break;
            }

            case CMD_MotorRotateStop :
            {
// 				printf("CMD_MotorRotateStop\r\n");
                MotorRotateStop_Send *send = (MotorRotateStop_Send *)ele.send;
                MotorRotateStop_Return *rt = New_MotorRotateStop_Return();
                rt->m1_errCode = motor_rotate_stop(&motors[send->m1_idx]);
                temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
                int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                free(data);
                Delete_MotorRotateStop_Send(send);
                Delete_MotorRotateStop_Return(rt);
                break;
            }
            
            case CMD_MotorMoveEven :////MotorMoveEvent = 0x24
            {
// 				printf("CMD_MotorMoveEven\r\n");
                MotorMoveEvent_Send *send = (MotorMoveEvent_Send*)ele.send;
                MotorMoveEvent_Return *rt = New_MotorMoveEvent_Return();
                 printf("i %d evi=%d\r\n",send->m1_idx,send->m6_eventId);
                motor_start_listen_event(&motors[send->m1_idx],true,send->m6_eventId,&motor_event_check_start);
                if(send->m5_isRelative)
                {
                    rt->m1_errCode = motor_move(&motors[send->m1_idx],send->m4_distance,send->m3_speed,send->m2_acce);
                }
                else
                {
                    rt->m1_errCode = motor_move_absolute(&motors[send->m1_idx],send->m4_distance,send->m3_speed,send->m2_acce,false);
                }
                motor_start_listen_event(&motors[send->m1_idx],false,send->m6_eventId,&motor_event_check_stop);
                rt->m2_trigger_pos = motors[send->m1_idx].event_triged_cord;
//                 printf("t pos = %d \r\n",rt->m2_trigger_pos);
                temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
                int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                if(rt->m1_errCode)
                {
                    printf("err %x\r\n",rt->m1_errCode);
                }
                free(data);
                Delete_MotorMoveEvent_Send(send);
                Delete_MotorMoveEvent_Return(rt);
                
                break;
            }
            
            case CMD_MotorMoveGPIOEvent : 
            {
// 				printf("CMD_MotorMoveGPIOEvent\r\n");
                MotorMoveGPIOEvent_Send *send = (MotorMoveGPIOEvent_Send*)ele.send;
                MotorMoveGPIOEvent_Return *rt = New_MotorMoveGPIOEvent_Return();
                GPIO_TypeDef * port = getPort((send->m6_state_port_pin>>4)&0x7);
                u16 pin = getPin(send->m6_state_port_pin &0xf);
                
                //初始化为输入
                GPIO_InitTypeDef io;
                memset(&io,0,sizeof(io));
                io.Pin = pin;
                io.Mode = GPIO_MODE_INPUT;
                io.Pull = GPIO_NOPULL;
                io.Speed = GPIO_SPEED_FAST;
                HAL_GPIO_Init(port, &io);
                motor_start_listen_io(&motors[send->m1_idx],send->m6_state_port_pin>>7,port,pin,readIo);
				
				motors[send->m1_idx].event_triged_cord = 0;
                if(send->m5_isRelative)
                {
                    rt->m1_errCode = motor_move(&motors[send->m1_idx],send->m4_distance,send->m3_speed,send->m2_acce);
                }
                else
                {
                    rt->m1_errCode = motor_move_absolute(&motors[send->m1_idx],send->m4_distance,send->m3_speed,send->m2_acce,false);
                }
                //关闭监听IO
                motor_start_listen_io(&motors[send->m1_idx],0,NULL,0,NULL);
                rt->m2_trigger_pos = motors[send->m1_idx].event_triged_cord;
//                 printf("t pos = %d \r\n",rt->m2_trigger_pos);
                temp_node = rt->ToBytes(rt);
                u8 *data = NULL;
                int len = BytesBufferNode_Destroy(temp_node,&data);
                bus_ita_return(ele.adapterId,ele.srcId,ele.sessionId,len,data);
                if(rt->m1_errCode)
                {
                    printf("err %x\r\n",rt->m1_errCode);
                }
                free(data);
                Delete_MotorMoveGPIOEvent_Send(send);
                Delete_MotorMoveGPIOEvent_Return(rt);
                
                break;
            }

        }
        
    }
}


/************************** 命令处理 *****************************************************************/

static void CmdHandle_SeqMotorultiMove(Motor_Queue_Ele_t *ele)
{
    printf("CmdHandle_SeqMotorultiMove\r\n");
    SeqMotorMultiMove_Send *send = (SeqMotorMultiMove_Send *)ele->send;
    SeqMotorMultiMove_Return *rt = New_SeqMotorMultiMove_Return();
    u32  err_ocr = 0;
    u32  err_idx = 0;
    for(int i =0;i<send->m1_Sequence_Length;i++)
    {
        SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs *mv = send->m1_Sequence[i].m1_MoveArgs;
        int mvCnt = send->m1_Sequence[i].m1_MoveArgs_Length;
        motor_multi_move_args_t * margs = malloc(sizeof(motor_multi_move_args_t)*mvCnt);
        
        for(int j = 0;j<mvCnt;j++)
        {
            margs[j].motor = &motors[   mv[j].m1_idx        ];
            margs[j].acce=              mv[j].m2_acce       ;
            margs[j].speed=             mv[j].m3_speed      ;
            margs[j].steps=             mv[j].m4_distance   ;
            margs[j].is_relative =      mv[j].m5_isRelative ;
            
//             printf("motor = %d \r\n",       mv[j].m1_idx       );
//             printf("acce  = %d \r\n",       mv[j].m2_acce      );
//             printf("speed = %d \r\n",       mv[j].m3_speed     );
//             printf("steps = %d \r\n",       mv[j].m4_distance  );
//             printf("is_relative = %d \r\n", mv[j].m5_isRelative);
        }
        
        motor_multi_move_absolute(mvCnt,margs,false);
        
        
        for(int j = 0;j<mvCnt;j++)
        {
            if(margs[j].errCode)
            {
                err_ocr = margs[j].errCode;
                err_idx = mv[j].m1_idx;
                break;
            }
        }
        free(margs);        //释放参数内存
        if(err_ocr == 0)
            bus_os_sleep(send->m1_Sequence[i].m2_delayms);

        if(err_ocr)
        {
            break;
        }

    }

    rt->m1_errCode = err_ocr;
    rt->m2_errIdx = err_idx;

    BytesBufferNode * temp_node = rt->ToBytes(rt);
    u8 *data = NULL;
    int len = BytesBufferNode_Destroy(temp_node,&data);
    bus_ita_return(ele->adapterId,ele->srcId,ele->sessionId,len,data);
    free(data);
    for(int i =0;i<send->m1_Sequence_Length;i++)
    {
        Delete_SeqMotorMultiMove_Send_m1_Sequence_m1_MoveArgs(send->m1_Sequence[i].m1_MoveArgs  );
    }
    Delete_SeqMotorMultiMove_Send_m1_Sequence(send->m1_Sequence );
    Delete_SeqMotorMultiMove_Send(send);
    Delete_SeqMotorMultiMove_Return(rt);
}

















