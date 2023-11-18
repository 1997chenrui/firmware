/**
************************************************************************************************
 * @file AutoAdapter.c
 * @brief 设置当前板卡的CAN网络地址.(写入特定位置的FLASH) 和读取当前 Local_ID
 * @verbatim
 使用: 调用函数: AutoAdapter_writeLocalID(uint32_t); 设置当前 ID
                 AutoAdapter_readLocalID();  读取现在ID为多少 //高效且高速的
 文件依存: 一定需要 flash.h 中 的 FirmWare_t 结构体大小 因为当先存储的值在 版本号 后面两个地址中
          同时需要注意的是,由于向片内flash中写入数据必须先擦除才能写入.所以.在擦除 FLASH_VERSION_SECTOR
          时需要注意保存其他数据.因此 在 flash_f4.c中有相关程序做了修改 @see FLASH_If_Erase(int sectorIdx);
       
 * @endverbatim
 * @author DESINPRO Co.,Ltd;
 * @version V0.0.5
 * @since 2019.02.28
 * @note CopyRight By DESINPRO Co. Ltd All right reserve 
 ***********************************************************************************************
*/
#include "AutoAdapter.h"
#include "stm32f4xx_hal.h"
#include "ErrCode.h"
#include "Protocols.h"

/**
 ***********************************************************************************************
 * @brief 此函数作为callback 函数调用接口.实现上位机 动态修改 板卡地址的需求.   \n
            接口函数传参为统一类型;                                             \n
          如果当前按键被按下,则认为当前需要修改自己的板卡ID                     \n
          如果修改成功.那么会返回 0 用于通知上位机设置成功.                     \n
          且会在500ms 内闪烁LED 灯 并重启.                                        
 * @param cmd                 指令种类.表明当前是那种指令.目前仅一种(ID_set)
 * @param packet              总线上传递过来的数据包(已经经过一层解析) . 
 * @return Return_Value_Type    Explanation_theReturn 
 * @author kimalittlestar@gmail.com
 * @note Remark_What_this_function_might_encounter
 **********************************************************************************************
 */
void AutoAdapter_Excute(int cmd,bus_packet *packet )
{
    static bool iscome = false;                                         
    if(iscome == false){//保证兼容性,从而不需要修改单个项目中的 main.c 文件中的代码
        iscome = true;
        GPIO_InitTypeDef GPIO_initstruct;
        GPIO_initstruct.Mode = GPIO_MODE_INPUT;
        GPIO_initstruct.Pin = AUTO_ADAPT_TRIGGER_PIN;
        GPIO_initstruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_initstruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(AUTO_ADAPT_TRIGGER_PORT,&GPIO_initstruct);
    }
    
    if((HAL_GPIO_ReadPin(AUTO_ADAPT_TRIGGER_PORT,AUTO_ADAPT_TRIGGER_PIN)!= 0))
        return ;            //检测到当前电平为高.直接返回(相关内存在bus.c 中被释放,无需关注)
            
    HAL_Delay(3);//延时消抖
    
    if((HAL_GPIO_ReadPin(AUTO_ADAPT_TRIGGER_PORT,AUTO_ADAPT_TRIGGER_PIN)!= 0))
        return ;            //再次检测
    
    //确定当前按键已经按下.开始ID更改
    AutoAdapter_Send *send = New_AutoAdapter_Send();
    AutoAdapter_Return *rt  =  New_AutoAdapter_Return();
    send->GetFromBytes(send,packet->data,4);

    rt->m1_writeState = (-1u);              //给定初始状态(按键未按下)
    uint32_t err = (-1u);
    
    err = AutoAdapter_writeLocalID(send->m1_localID);
    
    
    rt->m1_writeState = (uint32_t)err; 
    BytesBufferNode * node =  rt->ToBytes(rt);
    u8 *data = NULL;
    int len = BytesBufferNode_Destroy(node,&data);
    bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);

    free(data);
    Delete_AutoAdapter_Send(send);
    Delete_AutoAdapter_Return(rt);
    
    
    if(err == ADAPT_WRITE_OK)
    {//简单粗暴的双闪
        HAL_GPIO_WritePin(AUTO_ADAPT_LED1_GPIO,AUTO_ADAPT_LED1_PIN,GPIO_PIN_SET);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED2_GPIO,AUTO_ADAPT_LED2_PIN,GPIO_PIN_SET);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED3_GPIO,AUTO_ADAPT_LED3_PIN,GPIO_PIN_SET);
        OSTimeDly(100);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED1_GPIO,AUTO_ADAPT_LED1_PIN,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED2_GPIO,AUTO_ADAPT_LED2_PIN,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED3_GPIO,AUTO_ADAPT_LED3_PIN,GPIO_PIN_RESET);
        OSTimeDly(100);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED1_GPIO,AUTO_ADAPT_LED1_PIN,GPIO_PIN_SET);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED2_GPIO,AUTO_ADAPT_LED2_PIN,GPIO_PIN_SET);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED3_GPIO,AUTO_ADAPT_LED3_PIN,GPIO_PIN_SET);
        OSTimeDly(300);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED1_GPIO,AUTO_ADAPT_LED1_PIN,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED2_GPIO,AUTO_ADAPT_LED2_PIN,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AUTO_ADAPT_LED3_GPIO,AUTO_ADAPT_LED3_PIN,GPIO_PIN_RESET);
        NVIC_SystemReset();
    }
}



/**
 ***********************************************************************************************
 * @brief 将任意数值写入 一个固定的地址中(FLASH_VERSION_START+sizeof(FirmWare_t)+sizeof(uint32_t))                         \n
    作为当前的Can总线板卡地址
 * @param ID                 由上位机给定.届时会作为当前板卡的CAN网络ID 
 * @return AUTO_ADAPT_ERR_t    写数据的错误代码.如无错误则返回 0 @see AUTO_ADAPT_ERR_t
 * @author kimalittlestar@gmail.com
 * @note Remark_What_this_function_might_encounter
 **********************************************************************************************
 */
AUTO_ADAPT_ERR_t AutoAdapter_writeLocalID(uint32_t ID)
{
    ///< 其中有4步 1. 保存原有扇区其他数据. 2. 擦除扇区  3. 写入数据 4. 恢复其他数据.
    uint32_t err;
    
    uint32_t buf[sizeof(FirmWare_t)/sizeof(uint32_t) + 2];          //使用sizeof 强兼容,当前面的数据改动时,自行扩充数据
    uint32_t* pdata = (uint32_t*) FLASH_VERSION_START;
    for(int i = 0;i<(sizeof(buf)/sizeof(uint32_t));i++)             //使用uint32_t 作为单位.减少循环次数开销.
    {
        buf[i] = *pdata;
        pdata ++;
    }//数据备份
    
   
    {
        if(HAL_FLASH_Unlock() != HAL_OK)
            return ADAPT_UNLOCK_ERR;       ///< @note 届时应使用类型变量    pEraseInit.Banks = FLASH_BANK_1;
    }//解锁
    
    {
        FLASH_EraseInitTypeDef pEraseInit;
        HAL_StatusTypeDef status = HAL_OK;
        pEraseInit.NbSectors = 1;
        pEraseInit.Sector = FLASH_VERSION_SECTOR;
        pEraseInit.TypeErase  = FLASH_TYPEERASE_SECTORS;	//FLASH_TYPEERASE_SECTORS：区擦除    FLASH_TYPEERASE_MASSERASE：全擦除
        pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
        status = HAL_FLASHEx_Erase(&pEraseInit, &err);	//擦除,成功返回 HAL_OK
        if(status != HAL_OK)
            return ADAPT_ERASE_ERR;
     } //扇区擦除
    
    {
       for(int i = 0;i < (sizeof(buf)/sizeof(uint32_t));i++)
        {
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,FLASH_VERSION_START+i,buf[i]) != HAL_OK)
            {
                HAL_FLASH_Lock();
                return ADAPT_WRITE_ERR;
            }   
        } 
    }// 原有数据恢复
    
    
    {
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,FLASH_VERSION_START+sizeof(FirmWare_t)+sizeof(uint32_t),ID) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return ADAPT_WRITE_ERR;
        }
    }//写入数据且 上锁
    
    HAL_FLASH_Lock();
    return ADAPT_WRITE_OK;
    
}

/**
 ***********************************************************************************************
 * @brief 读取存在Flash中的自定义板卡号
 *
 * @return uint32_t    如果flash中的值为默认值,那么将返回(-1U); 
 * @author kimalittlestar@gmail.com
 * @note 如果当前返回值为-1 应该将板卡地址设置成一个默认值.并将此默认值写入Flash中 写入请调用 \n
 *                                                         AutoAdapter_writeLocalID(uint32_t)
 **********************************************************************************************
 */
inline uint32_t AutoAdapter_readLocalID(void )
{
    return *(uint32_t*)(FLASH_VERSION_START+sizeof(FirmWare_t)+sizeof(uint32_t));
}
