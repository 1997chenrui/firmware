#include "mTMC5130MotorHandle.h"
#include "cmd.h"
#include "ErrCode.h"
#include <stdlib.h>
#include "Protocols.h"

#include "mTMC5130DThread.h"
#include "mTMC5130Device.h"

extern  MOTOR motors[];
extern  motor_cfg_struct motors_cfg[];

#define DECLARE_SET_QUEUE_ELE(obj)  Motor_Queue_Ele_t ele;                    \
									ele.adapterId = packet->adapterId;        \
									ele.cmd = func_code;                    \
									ele.srcId = packet->srcId;                \
									ele.send = obj;                        \
									ele.sessionId = packet->sessionId;


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
		case CMD_DCMotorMoveTmc: //电机移动
		{
			DCMotorMoveTmc_Send *send = New_DCMotorMoveTmc_Send();
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
#if ENABLE_UPDATE_ZERO_MAKEUP_AND_ZERO_MODE == 1
				motors[send->m1_idx].mot_cfg->enable_zero_makeup=send->m8_isRotate & 1;
				motors[send->m1_idx].mot_cfg->back_zero_speed_mode=(send->m8_isRotate>>1)&1;
#endif
				motors[send->m1_idx].mot_cfg->lost_endure_continue=send->m4_lostEndureContinue;
				motors[send->m1_idx].mot_cfg->lost_endure_stop=send->m5_lostEndureStop;
				motors[send->m1_idx].mot_cfg->currentLevel=send->m11_currentLevel;			
				motors[send->m1_idx].mot_cfg->subdivision=send->m12_subdivision;
				motors[send->m1_idx].mot_cfg->holdLevel=send->m13_holdLevel;
				motors[send->m1_idx].mot_cfg->max_cordinate=send->m3_maxCordinate;
				motors[send->m1_idx].mot_cfg->zero_cordinate=send->m2_zeroCordinate;
				motors[send->m1_idx].mot_cfg->zero_trig_vol=send->m6_zeroTrigVol;

				printf("motors[%d].mot_cfg->dir_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->dir_vol);
				printf("motors[%d].mot_cfg->enb_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->enb_vol);
				printf("motors[%d].mot_cfg->half_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->half_vol);
				printf("motors[%d].mot_cfg->is_rotate=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->enable_zero_makeup);
				printf("motors[%d].mot_cfg->lost_endure_continue=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->lost_endure_continue);
				printf("motors[%d].mot_cfg->lost_endure_stop=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->lost_endure_stop);
				printf("motors[%d].mot_cfg->currentLevel=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->currentLevel);
				printf("motors[%d].mot_cfg->hold_currentLevel=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->holdLevel);
				printf("motors[%d].mot_cfg->subdivision=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->subdivision);
				printf("motors[%d].mot_cfg->max_cordinate=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->max_cordinate);
				printf("motors[%d].mot_cfg->zero_cordinate=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->zero_cordinate);
				printf("motors[%d].mot_cfg->zero_trig_vol=%d\r\n",send->m1_idx,motors[send->m1_idx].mot_cfg->zero_trig_vol);							

                BUS_ASSERT(send->m1_idx >= MOTOR_CNT,"motor idx error");
				mTMC5130Device_SetDefault(&motors[send->m1_idx]); //配置
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
		case CMD_SeqMotorMultiMove:      //MotorMultiMove
        {
            SeqMotorMultiMove_Send *send = New_SeqMotorMultiMove_Send();
            send->GetFromBytes(send, packet->data,4);
			DECLARE_SET_QUEUE_ELE(send);
			MotorThread_EnQueue(send->m1_Sequence[0].m1_MoveArgs[0].m1_idx ,&ele);					
            break;
        }		
        case CMD_MotorMoveSeq:
        {
            MotorMoveSeq_Send *send = New_MotorMoveSeq_Send();
            send->GetFromBytes(send, packet->data,4);
			DECLARE_SET_QUEUE_ELE(send);
			MotorThread_EnQueue(send->m1_MoveArgs[0].m1_idx ,&ele);					
            break;
        }
        case CMD_DCMotorMoveSeq:
        {
            DCMotorMoveSeq_Send *send = New_DCMotorMoveSeq_Send();
            send->GetFromBytes(send, packet->data,4);
			DECLARE_SET_QUEUE_ELE(send);
			MotorThread_EnQueue(send->m1_MoveArgs[0].m1_idx ,&ele);					
            break;
        }
        default :
        {
            while(1){printf("NoSupportThisCmd=0x%x\r\n",func_code);}
        }		
    }  
}

