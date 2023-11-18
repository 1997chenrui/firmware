#ifndef __EX_FLASH_H
#define __EX_FLASH_H


// W25Q16、W25Q32、W25Q64、W25Q128、W25Q256：Mbits
// One Page = 256B
// One Sector = 16Page = 4KB
// One Block = 16Sector = 64KB
// One Chip = 256Block = 16MB
// One Erase = 4KB or 32KB or 64KB

#define EX_FLASH_W25QXX						128			// 支持芯片型号：W25Q16、W25Q32、W25Q64、W25Q128、W25Q256
#define EX_FLASH_PAGE_SIZE					0x100		// 页大小：256B
#define EX_FLASH_SECTOR_SIZE				0x1000		// 扇区大小：4KB
#define EX_FLASH_BLOCK_SIZE					0x10000		// 块大小：64KB


// 用户参数存储区大小(字节)
#define EX_FLASH_USR_PARA_ADDR				(0x00000000)	// 起始地址
#define EX_FLASH_USR_PARA_SIZE				(100 * 1024)	// 参数存储区大小，单位：字节


// 0xEF13,表示芯片型号为W25Q80
// 0xEF14,表示芯片型号为W25Q16
// 0xEF15,表示芯片型号为W25Q32
// 0xEF16,表示芯片型号为W25Q64
// 0xEF17,表示芯片型号为W25Q128
typedef enum _eFlashType_t
{
	W25Err = 0x0000, // Unknow Type
	W25Q80 = 0xEF13, // 8MBit = 1MB; ID:0xEF13
	W25Q16 = 0xEF14, // 16MBit = 2MB; ID:0xEF14
	W25Q32 = 0xEF15, // 32MBit = 4MB; ID:0xEF15
	W25Q64 = 0xEF16, // 64MBit = 8MB; ID:0xEF16
	W25Q128 = 0xEF17, // 128MBit = 16MB; ID:0xEF17
	W25Q256 = 0xEF18, // 256MBit = 32MB; ID:0xEF18
}eFlashType_t;



/********************************************************
 * 函数功能：读取外部 Flash 型号
 * 形    参：无
 * 返 回 值：外部 Flash 型号
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
eFlashType_t exflash_type_get(void);


/********************************************************
 * 函数功能：读取外部 Flash 型号
 * 形    参：cs_pin：Flash片选引脚
 * 返 回 值：0=成功
             1=未知Flash型号
             2=内存申请失败
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
unsigned int exflash_init(unsigned int cs_pin);


/********************************************************
 * 函数功能：写外部Flash(自带擦除、自动换页的功能)
 * 形    参：addr：写入地址
             pbuf：数据缓存地址
             length：写入字节数
 * 返 回 值：0=成功
             1=数据异常
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
unsigned int exflash_write(unsigned int addr, const void *pbuf, unsigned int length);


/********************************************************
 * 函数功能：读外部Flash
 * 形    参：addr：写入地址
             pbuf：数据缓存地址
             length：写入字节数(不应该大于当前页的剩余字节数)
 * 返 回 值：0=成功
             1=数据缓存异常
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
unsigned int exflash_read(unsigned int addr, void * const pbuf, unsigned int length);


#endif
