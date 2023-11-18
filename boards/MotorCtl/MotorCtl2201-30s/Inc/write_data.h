#ifndef __WRITE_DATA_H
#define __WRITE_DATA_H



// 向总线主动发送数据，阻塞式发送
void bus_write_data(unsigned char *pbuf, unsigned short count, unsigned char adpId, unsigned char desId);

// 发送模块初始化
void bus_write_init(void);


#endif

