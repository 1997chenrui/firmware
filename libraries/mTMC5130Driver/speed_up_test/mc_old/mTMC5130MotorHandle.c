#include "mTMC5130MotorHandle.h"
#include "cmd.h"
#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"

#include "mTMC5130DThread.h"
#include "mTMC5130Device.h"

extern MOTOR motors[];
extern  motor_cfg_struct motors_cfg[];

#define DECLARE_SET_QUEUE_ELE(obj)  Motor_Queue_Ele_t ele;                    \
									ele.adapterId = packet->adapterId;        \
									ele.cmd = func_code;                    \
									ele.srcId = packet->srcId;                \
									ele.send = obj;                        \
									ele.sessionId = packet->sessionId;


static void mc_assert(bool exp,int line)
{
    if(exp)
    {
        while(1)
        {
            printf("cbk err %d \r\n",line);
        }
    }
}

#define MC_ASSERT( exp ) mc_assert((exp),__LINE__)


void MotorCmdExcute(u32 func_code  ,  bus_packet * packet )
{	  	
    switch(func_code)
    {
		case CMD_MotorMoveTmc: //电机移动
		{
			MotorMoveTmc_Send *send = New_MotorMoveTmc_Send();
			send->GetFromBytes(send,packet->data,4);
			DECLARE_SET_QUEUE_ELE(send);
			MotorThread_EnQueue(send->m1_idx,&ele);           			  
			break;
		}
		case CMD_MotorConfigSetTmc:  //参数配置
		{
			MotorConfigSetTmc_Send *send = New_MotorConfigSetTmc_Send();
			send->GetFromBytes(send,packet->data,4);
			MotorConfigSetTmc_Return *rt = New_MotorConfigSetTmc_Return();
			
            if(!motors[send->m1_idx].is_busy)
            {                 							
				motors[send->m1_idx].mot_cfg->dir_vol=send->m7_dirVol;
				motors[send->m1_idx].mot_cfg->enb_vol=send->m9_enableVol;
				motors[send->m1_idx].mot_cfg->half_vol=send->m10_halfVol;
				motors[send->m1_idx].mot_cfg->is_rotate=send->m8_isRotate;
				motors[send->m1_idx].mot_cfg->lost_endure_continue=send->m4_lostEndureContinue;
				motors[send->m1_idx].mot_cfg->lost_endure_stop=send->m5_lostEndureStop;
				motors[send->m1_idx].mot_cfg->currentLevel=send->m11_currentLevel;
				motors[send->m1_idx].mot_cfg->subdivision=send->m12_subdivision;
				motors[send->m1_idx].mot_cfg->max_cordinate=send->m3_maxCordinate;
				motors[send->m1_idx].mot_cfg->zero_cordinate=send->m2_zeroCordinate;
				motors[send->m1_idx].mot_cfg->zero_trig_vol=send->m6_zeroTrigVol;

				printf("motors[%d].mot_cfg->dir_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->dir_vol);
				printf("motors[%d].mot_cfg->enb_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->enb_vol);
				printf("motors[%d].mot_cfg->half_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->half_vol);
				printf("motors[%d].mot_cfg->is_rotate=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->is_rotate);
				printf("motors[%d].mot_cfg->lost_endure_continue=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->lost_endure_continue);
				printf("motors[%d].mot_cfg->lost_endure_stop=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->lost_endure_stop);
				printf("motors[%d].mot_cfg->currentLevel=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->currentLevel);
				printf("motors[%d].mot_cfg->subdivision=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->subdivision);
				printf("motors[%d].mot_cfg->max_cordinate=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->max_cordinate);
				printf("motors[%d].mot_cfg->zero_cordinate=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->zero_cordinate);
				printf("motors[%d].mot_cfg->zero_trig_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->zero_trig_vol);							
//              Flash_SetObject( &motors_cfg,sizeof(motors_cfg) );            
//              MotorInit(send->m1_idx);
                rt->m1_errCode = 0;
            }
            else
            {
				printf("		busy\r\n");
				rt->m1_errCode = ERR_MOTOR_BUSY;
            }
			
			u8 *data = NULL;
			BytesBufferNode * node =  rt->ToBytes(rt);

			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			free(data);
			Delete_MotorConfigSetTmc_Send(send);
			Delete_MotorConfigSetTmc_Return(rt);
			break;
		}	
		case CMD_MotorMoveSyncLevel:	///< 抓手与采值同步信号	
		{
            MotorMoveSyncLevel_Send *send = New_MotorMoveSyncLevel_Send();			
			send->GetFromBytes(send,packet->data,4);
			DECLARE_SET_QUEUE_ELE(send);
			MotorThread_EnQueue(send->m1_idx,&ele);           			  		
			break;
		}			
        case CMD_MotorRotateStartTmc:	//旋转开始		
		{
			MotorRotateStartTmc_Send *send = New_MotorRotateStartTmc_Send();
			send->GetFromBytes(send,packet->data,4);
			DECLARE_SET_QUEUE_ELE(send);
			MotorThread_EnQueue(send->m1_idx,&ele);           			  		
			break;
		}	
        case CMD_MotorRotateStopTmc:	//旋转结束
		{
			MotorRotateStopTmc_Send *send = New_MotorRotateStopTmc_Send();
			send->GetFromBytes(send,packet->data,4);
			DECLARE_SET_QUEUE_ELE(send);
			MotorThread_EnQueue(send->m1_idx,&ele);           			  		
			break;
		}	
        case CMD_MotorResetTmc:
        {         
            MotorResetTmc_Send *reset = New_MotorResetTmc_Send();
            reset->GetFromBytes(reset, packet->data,4);
            DECLARE_SET_QUEUE_ELE(reset);
            MotorThread_EnQueue(reset->m1_idx ,&ele);           
            break;
        }			
        case CMD_MotorMoveGPIOEventTmc:
		{
            MotorMoveGPIOEventTmc_Send *send = New_MotorMoveGPIOEventTmc_Send();
            send->GetFromBytes(send,packet->data,4);
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx ,&ele);       
            break;					
		}
        case CMD_MotorStatetGetTmc:
		{
            MotorStatetGetTmc_Send *send = New_MotorStatetGetTmc_Send();
            send->GetFromBytes(send,packet->data,4);
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx ,&ele);       
            break;
		}					
		case CMD_MotorMultiMoveTmc:      //MotorMultiMove
        {
            MotorMultiMoveTmc_Send *send = New_MotorMultiMoveTmc_Send();
            send->GetFromBytes(send, packet->data,4);
			DECLARE_SET_QUEUE_ELE(send);
			MotorThread_EnQueue(send->m1_MoveArgs[0].m1_idx ,&ele);					
            break;
        }	
/*................................................................上面是5130驱动器的命令码...................................*/		
        case CMD_MotorReset:
        {          
            MotorReset_Send *reset = New_MotorReset_Send();
            reset->GetFromBytes(reset, packet->data,4);
            DECLARE_SET_QUEUE_ELE(reset);
            MotorThread_EnQueue(reset->m1_idx ,&ele);           
            break;
        }     
        case CMD_MotorMove:
        {
            MotorMove_Send *send = New_MotorMove_Send();
            send->GetFromBytes(send,packet->data,4);
            MC_ASSERT(send->m1_idx >= 5);
//            MC_ASSERT(send->m2_acce > 80*10000 );
//            MC_ASSERT(send->m3_speed >= 4*10000 );
            MC_ASSERT(send->m5_isRelative != 0 && send->m5_isRelative != 1 );         
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx,&ele);          
            break;
        }
        case CMD_MotorRotateStart :
        {
            MotorRotateStart_Send *send = New_MotorRotateStart_Send();
            send->GetFromBytes(send,packet->data,4);
            
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx,&ele);
            printf("rot st %d \r\n",send->m1_idx);
            break;
        }
     
        case CMD_MotorRotateStop :
        {
            MotorRotateStop_Send *send = New_MotorRotateStop_Send();
            send->GetFromBytes(send,packet->data,4);
            
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx,&ele);
            break;
        }

        case CMD_MotorAbort :
        {
            printf("CMD_MotorAbort");
            MotorAbort_Send *send = New_MotorAbort_Send();
            MotorAbort_Return *rt = New_MotorAbort_Return();
            send->GetFromBytes(send,packet->data,4);
            printf("AB=%x\r\n",send->m1_idx_mask);
            for(int i =0;i<3;i++)
            {
                if(send->m1_idx_mask & (1<<i))
                {
                    motors[i].is_abort = true;  //只有忙才需要急停
                    printf("%d abt\r\n",i);
                }

            }
            
            u8 *data = NULL;
            rt->m1_errCode = 0;
            BytesBufferNode * node =  rt->ToBytes(rt);
            int len = BytesBufferNode_Destroy(node,&data);
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
            free(data);
            Delete_MotorAbort_Send(send);
            Delete_MotorAbort_Return(rt);
            break;
        }

        case CMD_MotorMultiMove:    
        {
            MotorMultiMove_Send *send = New_MotorMultiMove_Send();
            send->GetFromBytes(send, packet->data,4);
            MC_ASSERT(send->m1_MoveArgs_Length > 3);
			
            for(int i =0;i<send->m1_MoveArgs_Length;i++)
            {
                MC_ASSERT(send->m1_MoveArgs[i].m1_idx >= 5);
//                MC_ASSERT(send->m1_MoveArgs[i].m2_acce > 80*10000);
//                MC_ASSERT(send->m1_MoveArgs[i].m3_speed >= 6*10000);
                MC_ASSERT(send->m1_MoveArgs[i].m5_isRelative != 1 && send->m1_MoveArgs[i].m5_isRelative != 0  );
            }
            
            DECLARE_SET_QUEUE_ELE(send);
            int idx = send->m1_MoveArgs[0].m1_idx;
            MotorThread_EnQueue(idx ,&ele);
            break;
        }     
        case CMD_MotorStatetGet :
        {
            MotorStatetGet_Send *send = New_MotorStatetGet_Send();
            send->GetFromBytes(send,packet->data,4);
            
            MC_ASSERT(send->m1_idx >= 5);
            
            MotorStatetGet_Return *rt = New_MotorStatetGet_Return();
            		
            rt->m1_isZeroTriggered      = !motors[send->m1_idx].read_io(READ_PIN_ID_SENSOR,0,0);
            rt->m2_currentCordinate     = mTMC5130DeviceGetPostion(&motors[send->m1_idx],1);
            rt->m3_lostSteps            =   0;
            rt->m4_isBusy               =    0;
						
            BytesBufferNode * node =  rt->ToBytes(rt);
            u8 *data = NULL;
            int len = BytesBufferNode_Destroy(node,&data);
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
            free(data);
            Delete_MotorStatetGet_Send(send);
            Delete_MotorStatetGet_Return(rt);
            break;
        }		
		case CMD_MotorMoveGPIOEvent:
        {
            MotorMoveGPIOEvent_Send *send = New_MotorMoveGPIOEvent_Send();
            send->GetFromBytes(send,packet->data,4);
            DECLARE_SET_QUEUE_ELE(send);
            MotorThread_EnQueue(send->m1_idx,&ele);
            break;
        }
        
        case CMD_MotorConfigSet: 
        {
            //TODO 判断是不是忙
            MotorConfigSet_Send *send = New_MotorConfigSet_Send();
            send->GetFromBytes(send,packet->data,4);
            MotorConfigSet_Return *rt = New_MotorConfigSet_Return();
            u8 *data = NULL;
//            if(!motors[send->m1_idx].is_busy)
//            {
//                //不忙才能改配置
//                motors[send->m1_idx].mot_cfg->zero_cordinate=          send->m2_zeroCordinate;
//                motors[send->m1_idx].mot_cfg->max_cordinate=           send->m3_maxCordinate;
//                motors[send->m1_idx].mot_cfg->lost_endure_continue=    send->m4_lostEndureContinue;
//                motors[send->m1_idx].mot_cfg->lost_endure_stop=        send->m5_lostEndureStop;
//                motors[send->m1_idx].mot_cfg->zero_trig_vol=           send->m6_zeroTrigVol;
//                motors[send->m1_idx].mot_cfg->dir_vol=                 send->m7_dirVol;
//                motors[send->m1_idx].mot_cfg->is_rotate=               send->m8_isRotate;
//                motors[send->m1_idx].mot_cfg->enb_vol=                 send->m9_enableVol;
//                motors[send->m1_idx].mot_cfg->half_vol=                send->m10_halfVol;
//                
//				printf("motor%d:zero_trig_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->zero_trig_vol);
//				printf("motor%d:dir_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->dir_vol);
//				printf("motor%d: zero_cordinate= %d \r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->zero_cordinate);
//				printf("motor%d:max_cordinate= %d \r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->max_cordinate);
//				printf("motor%d:lost_endure_continue= %d \r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->lost_endure_continue);
//				printf("motor%d:lost_endure_stop= %d \r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->lost_endure_stop);
//                rt->m1_errCode = 0;
//            }
//            else
//            {
//				printf("		busy\r\n");
//                rt->m1_errCode = ERR_MOTOR_BUSY;
//            }

			BytesBufferNode * node =  rt->ToBytes(rt);			
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
            free(data);
            Delete_MotorConfigSet_Send(send);
            Delete_MotorConfigSet_Return(rt);          
            break;
        }       
		case CMD_MotorConfigSetNew :
        {
            MotorConfigSetNew_Send *send = New_MotorConfigSetNew_Send();
            send->GetFromBytes(send,packet->data,4);
            MotorConfigSetNew_Return *rt = New_MotorConfigSetNew_Return();
			
            u8 *data =NULL;
			
//            int idx = send->m1_idx_zero_dir_isR_en_hf & 0x7;
//            int zero = (send->m1_idx_zero_dir_isR_en_hf>>3) &0x1;
//            int dir = (send->m1_idx_zero_dir_isR_en_hf>>4) &0x1;
//            int isR = (send->m1_idx_zero_dir_isR_en_hf>>5) &0x1;
//            int en = (send->m1_idx_zero_dir_isR_en_hf>>6) &0x1;
//            int hf = (send->m1_idx_zero_dir_isR_en_hf>>7) &0x3;
            
//            if(!motors[idx].is_busy)
//            {  
//                motors[idx].mot_cfg->zero_cordinate=          send->m3_zeroCordinate;
//                motors[idx].mot_cfg->max_cordinate=           send->m2_maxCordinate; 
//                motors[idx].mot_cfg->lost_endure_continue=    send->m4_lostEndureContinue; 
//                motors[idx].mot_cfg->lost_endure_stop=        send->m5_lostEndureStop;
//                motors[idx].mot_cfg->zero_trig_vol=           zero;    
//                motors[idx].mot_cfg->dir_vol=                 dir;
//                motors[idx].mot_cfg->is_rotate=               isR;
//                motors[idx].mot_cfg->enb_vol=                 en;
//                motors[idx].mot_cfg->half_vol=                hf;

//                rt->m1_errCode = 0;
//                BytesBufferNode * node =  rt->ToBytes(rt);
//                
//                int len = BytesBufferNode_Destroy(node,&data);
//                bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
//            }
//            else
//            {
//                rt->m1_errCode = ERR_MOTOR_BUSY;
//            }
			
			BytesBufferNode * node =  rt->ToBytes(rt);		
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);			
            free(data);
            Delete_MotorConfigSetNew_Send(send);
            Delete_MotorConfigSetNew_Return(rt);
            break; 
        }
        default :
        {
            printf("NoSupportThisCmd=0x%x\r\n",func_code);
            break;
        }		
    }  
}

