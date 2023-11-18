#ifndef _AUTO_ADAPTER_H_
#define _AUTO_ADAPTER_H_
#include "flash.h"              
#include "bus.h"

#define AUTO_ADAPT_LED1_GPIO         GPIOB                  ///< 当设置ID成功时此灯会闪烁
#define AUTO_ADAPT_LED1_PIN          GPIO_PIN_2

#define AUTO_ADAPT_LED2_GPIO         GPIOC                  ///< 当设置ID成功时此灯会闪烁
#define AUTO_ADAPT_LED2_PIN          GPIO_PIN_4

#define AUTO_ADAPT_LED3_GPIO         GPIOC                  ///< 当设置ID成功时此灯会闪烁
#define AUTO_ADAPT_LED3_PIN          GPIO_PIN_5
        
#define AUTO_ADAPT_TRIGGER_PORT      GPIOC                  ///< 只有在这个 按键IO按下(拉低) 时 CMD_AutoAdapterSet 命令有效
#define AUTO_ADAPT_TRIGGER_PIN       GPIO_PIN_14

typedef enum
{
// Edit enum_menber;
    ADAPT_UNLOCK_ERR = 1,                     ///< 写片内FLSH时 , 解锁出错
    ADAPT_LOCK_ERR   = 2,                     ///< 写片内FLSH时 , 上锁出错
    ADAPT_WRITE_ERR  = 3,                     ///< 写片内FLSH时 , 擦除出错
    ADAPT_ERASE_ERR  = 4,                     ///< 写片内FLSH时 , 写入数据出错
    ADAPT_WRITE_OK   = 0                      ///< 正常返回值
    
} AUTO_ADAPT_ERR_t;        // flash write error type define

AUTO_ADAPT_ERR_t AutoAdapter_writeLocalID(uint32_t ID);
uint32_t AutoAdapter_readLocalID(void);
void AutoAdapter_Excute(int cmd,bus_packet *packet );                   // 如果这里报错 请在项目中添加AutoAdapter.c 文件 common/Protocols 文件夹中


#endif
