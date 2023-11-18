#include "bus.h"
#include "itf_01000920.h"
#include "sp_light.h"

void signal_cmd_extend(u32 itf_code, bus_packet *packet)
{
	Itf_01000920_t* cmd = itf_01000920_new();
	cmd->packet_parse(cmd, packet);
	for(int i=0;i<8;i++)
	{
//		wave[i].need_updata = 1;
	}
	cmd->rtn_and_free(cmd, packet,0);
}


u8 bus_ita_func_callback_extern(u32 func_code, bus_packet *packet)
{
	switch (func_code)
	{
	case 0x01000920:
	{
		signal_cmd_extend(func_code, packet);
		break;
	}
	default:
		return 1;
	}
	return 0;
}
