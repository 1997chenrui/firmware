#include "itf_00050000.h"
#include "bus.h"
#include "tool.h"
#include <stdlib.h>

extern u8 MyAdapterID;
static void rtn_and_content_del(Itf_00050000_content_t *content, u8 error_code);
static Itf_00050000_content_t *packet_parse(bus_packet *packet);
static void send(u8 id, u8 *data, u32 len);

Itf_00050000_t* Itf_00050000_new(void) {
    Itf_00050000_t *self = malloc(sizeof(Itf_00050000_t));
    self->packet_parse = packet_parse;
    self->rtn_and_content_del = rtn_and_content_del;
    self->send = send;
    return self;
}

static Itf_00050000_content_t *packet_parse(bus_packet *packet) {
    u8 offset = 4;
    Itf_00050000_content_t *content =
        (Itf_00050000_content_t *)malloc(sizeof(Itf_00050000_content_t));
    content->id = packet->data[offset];
    offset++;
    content->len = ReadU32(&packet->data[offset]);
    offset += 4;
    content->data = malloc(content->len);
    memmove(content->data, &packet->data[offset], content->len);
    content->packet = *packet;
    content->packet.data = NULL;
    return content;
}

static void content_del(Itf_00050000_content_t *content) {
    free(content->data);
    free(content);
}

static void rtn_and_content_del(Itf_00050000_content_t *content,
                                u8 error_code) {
    u8 data = 0;
    BUS_ITA_RTN(content->packet, &data, 1);
    content_del(content);
}

static void send(u8 id, u8 *data, u32 len) {
    u8 *array = malloc(len + 9);
    u8 offset = 0;
    WriteU32(array, 0x00050000);
    offset+=4;
    array[offset++] = id;
    WriteU32(&array[offset], len);
    offset += 4;
    memmove(&array[offset], data, len);
    offset+=len;
    
    u8 data_out;
    u32 data_out_len;
	
	printf("new uart data upload:%d\r\n",len);
	for(u16 i=0;i<len;i++)
	{
		printf("%c ",data[i]);
	}
	printf("\r\n");
	
    bus_ita_hb_send(MyAdapterID, 0, len + 9, array, &data_out, &data_out_len);
    free(array);
}
