#include "callback.h"
#include "protocols.h"
#include "motor.h"
#include "ErrCode.h"
#include "io_operation.h"
#include "cmd.h"
#include "flash.h"

 


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
	
	FirmWare_Excute(func_code,packet);
    
    return 0;
}




u32 bus_ita_func_broadcast(bus_packet * packet)
{
    printf("bus_ita_func_broadcast\r\n");
    return 0;
}

//u8 send_data[200] = {
//    1,2,3,4,5,6,7,8,9,10,111,222,

//};


