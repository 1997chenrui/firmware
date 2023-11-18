#include "boot_loader_callback.h"
#include "protocols.h"
#include "ErrCode.h"
#include "cmd.h"
#include "flash.h"
#include "CommonHandle.h"
 
extern bus_adapter CanAdapter;

#define DECLARE_SET_QUEUE_ELE(obj)              Motor_Queue_Ele_t ele;                    \
                                                ele.adapterId = packet->adapterId;        \
                                                ele.cmd = func_code;                    \
                                                ele.srcId = packet->srcId;                \
                                                ele.send = obj;                        \
                                                ele.sessionId = packet->sessionId;
												

u32 bus_ita_func_callback(bus_packet * packet)
{
	u32 func_code = 0;
	func_code = ReadU32(packet->data);
	
    switch(func_code)
    {
        case CMD_FirmWareConfig :    
        case CMD_FirmWareTransmitFile  :   
        case CMD_McuReset        :         
        case CMD_FirmWareVersionGet     :  
        case CMD_FirmWareReadyEvent    :   
            FirmWare_Excute(func_code,packet);
			break;
        
        case CMD_Ping :
            CommonCmdExcute(func_code,  packet );
			break;
		default:
		{
			bus_ita_return_unkonw_cmd(packet);
			break;
		}
    }
	
    
    return 0;
}



u32 bus_ita_func_broadcast(bus_packet * packet)
{
    printf("bus_ita_func_broadcast\r\n");
    return 0;
}



