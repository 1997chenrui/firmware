#include "CommonHandle.h"
#include "cmd.h"
#include <stdlib.h>



void CommonCmdExcute(u32 cmd, bus_packet *packet)
{
    switch(cmd)
    {
        case CMD_Ping:
        {
            u8 rt[16] = {0};
            WriteU32(rt, 12);//前四字节写入12
            
			#ifdef STM32F407xx         
            u32 * base = (u32 *)( 0x1FFF7A10);
			#else
            u32 * base = (u32 *)( 0x1FFFF7E8);
			#endif
            
            memmove(rt + 4, base, 12); // 复制唯一ID
			
            bus_ita_return(packet->adapterId, packet->srcId, packet->sessionId, 16, rt);
			break;
        }
		default:break;
    }
}

