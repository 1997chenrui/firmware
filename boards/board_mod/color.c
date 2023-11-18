#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "bus.h"
#include "tool.h"
#include "itf_00080010.h"
#include "itf_00080011.h"
#include "itf_00080012.h"
#include "color_cfg.h"

extern ColorMod_t color[];

/***外部函数***/
void colormod_init(){
	color_hd_init();
}

ColorMod_t* colormod_get_as_ref(u8 idx){
    return &color[idx];
}

static Ws2812b_color_u convert(u32 color_value){
	Ws2812b_color_u color;
	color.value = 0;
	color.rgb.r = (color_value >> 16) & 0xff;
	color.rgb.g = (color_value >> 8) & 0xff;
	color.rgb.b = (color_value) & 0xff;
	return color;
}

void ws_led_cmd_run(u32 itf_code, bus_packet* packet){
	ColorMod_t* self = NULL;
    switch(itf_code){
        case 0x00080010:{
            Itf_00080010_content_t content = Itf_00080010_parse(packet);
			self = &color[content.group_id];
			Ws2812b_color_u color = convert(content.color);
			self->ws->color_single_write(self->ws, content.id, color); 
            Itf_00080010_rtn(packet, 0);
			break;
        }
        case 0x00080011:{
			Itf_00080011_content_t content = Itf_00080011_parse(packet);
			self = &color[content.group_id];
			Ws2812b_color_u color = convert(content.color);
			self->ws->color_select_write(self->ws, content.id, color); 
			Itf_00080011_rtn(packet, 0);
            break;
        }
		case 0x00080012:{
            Itf_00080012_content_t content = Itf_00080012_parse(packet);
			self = &color[content.group_id];
			Ws2812b_color_u color = convert(content.color);
			self->ws->color_select_write(self->ws, content.id, color);
			Itf_00080012_rtn(packet, 0);
            break;
        }
        default:ASSERT(1, "io cmd no support");
    }
}
