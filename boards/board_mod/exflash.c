#include "exflash.h"
//#include "mcu.h"
#include "gpio.h"
#include "stm32f4xx.h"


#define W25X_ReadData			0x03 // 读数据
#define W25X_WriteDisable		0x04 // 写禁能
#define W25X_WriteEnable		0x06 // 写使能

#define W25X_WriteStatusReg		0x01 // 写状态寄存器
#define W25X_ReadStatusReg1		0x05 // 读状态寄存器1
#define W25X_ReadStatusReg2		0x35 // 读状态寄存器2

#define W25X_PageProgram		0x02 // 页编程命令
#define W25X_BlockErase_32K		0x52 // 32K块擦除
#define W25X_BlockErase_64K		0xD8 // 64K块擦除
#define W25X_SectorErase		0x20 // 扇区擦除
#define W25X_ChipErase			0xC7 // 芯片擦除，指令也可能是0x60

#define W25X_PowerOn			0xAB // 退出掉电模式
#define W25X_PowerDown			0xB9 // 进入掉电模式
#define W25X_DeviceID			0x90 // 读取设备ID

// SPI总线最大设备个数
#define SPI_DEVICE_COUNT	((unsigned char)10)
#define SPI_DEVICE_ENABLE	((unsigned char)0)
#define SPI_DEVICE_DISABLE	((unsigned char)1)
	
extern SPI_HandleTypeDef hspi1;

unsigned char spi_rw(unsigned int cs_pin, unsigned char tx_data)
{
	cs_pin = 0;
	while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET) {};
	hspi1.Instance->DR = tx_data;
	while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET) {};
	cs_pin = 1;
	return hspi1.Instance->DR;
}

	
#define FLASH_CSN(en)			gpio_write_pin(exFlashCsPin, en)
#define FLASH_READ()			spi_rw(exFlashCsPin, 0xFF)
#define FLASH_WRITE(data)		spi_rw(exFlashCsPin, data)


static unsigned int exFlashCsPin = 0; // 外部Flash片选
static eFlashType_t eFlashType = W25Err; // 外部 Flash 型号
static unsigned char FlashReadBuff[EX_FLASH_SECTOR_SIZE] = {0}; // 缓存区



//=====================================================================================================
//                                      Flash Read Function
//=====================================================================================================



/********************************************************
 * 函数功能：读外部 FLASH 寄存器
 * 形    参：无
 * 返 回 值：状态寄存器值
             BIT7  6   5   4   3   2   1   0
			 SPR:默认0,状态寄存器保护位,配合WP使用
			 TB,BP2,BP1,BP0:FLASH区域写保护设置
			 WEL:写使能锁定
			 BUSY:忙标记位(1:忙; 0:空闲)
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
static unsigned char exflash_read_reg(void)
{
	unsigned char status = 0;
	
	FLASH_CSN(SPI_DEVICE_ENABLE); // 片选使能
	FLASH_WRITE(W25X_ReadStatusReg1); // 发送读状态寄存器命令
	status = FLASH_READ(); // 读一个字节
	
	// 注：如果不拉高CS，寄存器的值将会一直从DO引脚输出
	FLASH_CSN(SPI_DEVICE_DISABLE); // 拉高CS，结束读指令
	
	return status;
}


/********************************************************
 * 函数功能：外部 FLASH 忙等待
 * 形    参：无
 * 返 回 值：1=timeout; 0=idle
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
static unsigned char exflash_busy_wait(void)
{
	unsigned short int timeout = 0;
	
	// 等待 Flash 写结束
	while ((exflash_read_reg() & 0x01) != 0x00)
	{
		if(timeout++ > 0xFFF0)
		{
			return 1;
		}
	}
	
	return 0;
}


/********************************************************
 * 函数功能：读外部Flash设备ID
 * 形    参：无
 * 返 回 值：外部 FLASH 的 ID
             0xEF13,表示芯片型号为W25Q80
             0xEF14,表示芯片型号为W25Q16
             0xEF15,表示芯片型号为W25Q32
             0xEF16,表示芯片型号为W25Q64
             0xEF17,表示芯片型号为W25Q128
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
static eFlashType_t exflash_read_device_id(void)
{
	unsigned short id = 0;
	
	FLASH_CSN(SPI_DEVICE_ENABLE); // 片选使能
	FLASH_WRITE(W25X_DeviceID); // 发送读 Flash ID 的命令
	
	// 发送地址，addr=0:先接收生产ID后接收器件ID，addr=1:接收顺序颠倒
	if(eFlashType == W25Q256)
	{
		FLASH_WRITE(0x00); // 32bit addr
	}
	
	FLASH_WRITE(0x00);
	FLASH_WRITE(0x00);
	FLASH_WRITE(0x00);
	
	// Flash ID 读取
	id = (id << 8) + FLASH_READ(); // Manufacturer ID (EFH)
	id = (id << 8) + FLASH_READ(); // Device ID
	
	FLASH_CSN(SPI_DEVICE_DISABLE); // 片选禁能
	return (eFlashType_t)id;
}


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
unsigned int exflash_read(unsigned int addr, void * const pbuf, unsigned int length)
{
	unsigned char *pdata = (unsigned char *)pbuf;
	
	if(!pbuf || length < 1)
	{
		return 1;
	}
	
	gpio_write_pin(exFlashCsPin, SPI_DEVICE_ENABLE); // 片选使能
	FLASH_WRITE(W25X_ReadData); // 发送读数据命令
	
	// 发送数据起始地址
	if(eFlashType == W25Q256)
	{
		FLASH_WRITE((unsigned char)(addr >> 24)); // 32bit addr
	}
	FLASH_WRITE((unsigned char)(addr >> 16));
	FLASH_WRITE((unsigned char)(addr >> 8));
	FLASH_WRITE((unsigned char)(addr >> 0));
	
	// 开始读数据
	for(unsigned int i = 0; i < length; i++)
	{
		pdata[i] = FLASH_READ();
	}
	
	gpio_write_pin(exFlashCsPin, SPI_DEVICE_DISABLE); // 片选禁能
	return 0;
}


//=====================================================================================================
//                                      Flash Write Function
//=====================================================================================================



/********************************************************
 * 函数功能：外部 FLASH 扇区擦除(Erase time: 150ms)
 * 形    参：sector_num：扇区号
 * 返 回 值：无
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
static void exflash_sector_erase(unsigned int sector_num)
{
	// 1 sector = 4KB
	unsigned int addr = sector_num << 12; // 乘 4096
	
	FLASH_CSN(SPI_DEVICE_ENABLE); // 片选使能
	FLASH_WRITE(W25X_WriteEnable);
	FLASH_CSN(SPI_DEVICE_DISABLE); // 片选禁能
	exflash_busy_wait(); // 等待 Flash 空闲
	FLASH_CSN(SPI_DEVICE_ENABLE); // 片选使能
	FLASH_WRITE(W25X_SectorErase); // 发送扇区擦除指令
	
	// 发送擦除起始地址
	if(eFlashType == W25Q256)
	{
		FLASH_WRITE((unsigned char)(addr >> 24)); // 32bit addr
	}
	FLASH_WRITE((unsigned char)(addr >> 16));
	FLASH_WRITE((unsigned char)(addr >> 8));
	FLASH_WRITE((unsigned char)(addr >> 0));
	
	FLASH_CSN(SPI_DEVICE_DISABLE); // 片选禁能
	exflash_busy_wait(); // 等待 Flash 擦除结束
}


/********************************************************
 * 函数功能：写外部Flash页内存块
 * 形    参：addr：写入地址
             pbuf：数据缓存地址
             length：写入字节数(不应该大于当前页的剩余字节数)
 * 返 回 值：0=成功
             1=数据异常
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
static unsigned int exflash_write_page(unsigned int addr, void * const pbuf, unsigned int length)
{
	unsigned char *pdata = (unsigned char *)pbuf;
	
	if(!pdata || length < 1 || length > EX_FLASH_PAGE_SIZE)
	{
		return 1;
	}
	
	FLASH_CSN(SPI_DEVICE_ENABLE); // 片选使能
	FLASH_WRITE(W25X_WriteEnable);
	FLASH_CSN(SPI_DEVICE_DISABLE); // 片选禁能
	exflash_busy_wait(); // 等待 Flash 空闲
	FLASH_CSN(SPI_DEVICE_ENABLE); // 片选使能
	FLASH_WRITE(W25X_PageProgram); // 发送页写命令
	
	// 发送数据起始地址
	if(eFlashType == W25Q256)
	{
		FLASH_WRITE((unsigned char)(addr >> 24)); // 32bit addr
	}
	FLASH_WRITE((unsigned char)(addr >> 16));
	FLASH_WRITE((unsigned char)(addr >> 8));
	FLASH_WRITE((unsigned char)(addr >> 0));
	
	// 开始写数据
	for(unsigned int i = 0; i < length; i++)
	{
		FLASH_WRITE(pdata[i]);
	}
	
	FLASH_CSN(SPI_DEVICE_DISABLE); // 片选禁能
	return exflash_busy_wait(); // 等待 Flash 写结束
}


/********************************************************
 * 函数功能：写外部Flash扇区
 * 形    参：addr：写入地址
             pbuf：数据缓存地址
             length：写入字节数
 * 返 回 值：0=成功
             1=数据异常
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
static unsigned int exflash_write_sector(unsigned int addr, void * const pbuf, unsigned int length)
{
	unsigned int write_addrs = 0;
	unsigned int write_count = 0; // 记录总字节数
	unsigned int page_remain = 0; // 单页剩余字节数
	unsigned char *pdata = (unsigned char *)pbuf;
	
	if(!pdata || length < 1 || length > EX_FLASH_SECTOR_SIZE)
	{
		return 1;
	}
	
	write_addrs = addr;
	write_count = length;
	page_remain = EX_FLASH_PAGE_SIZE - addr % EX_FLASH_PAGE_SIZE;
	if(write_count <= page_remain)
	{
		page_remain = write_count;
	}
	
	while(1)
	{
		if(exflash_write_page(write_addrs, pdata, page_remain) != 0)
		{
			return 1;
		}
		
		if(page_remain == write_count)
		{
			break; // 写入完毕
		}
		
		pdata += page_remain; // 数据指针偏移
		write_addrs += page_remain; // 写入地址偏移
		write_count -= page_remain; // 更新待写字节总数
		page_remain = (write_count > EX_FLASH_PAGE_SIZE)? EX_FLASH_PAGE_SIZE : write_count;
	}
	
	return 0;
}


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
unsigned int exflash_write(unsigned int addr, const void *pbuf, unsigned int length)
{
	unsigned int i = 0;
	unsigned int write_addr = addr;
	unsigned int write_count = length;
	unsigned int sector_number = 0; // 扇区号
	unsigned int sector_offset = 0; // 扇区内偏移量
	unsigned int sector_remain = 0; // 扇区内剩余空间
	unsigned int wr_base_addrs = 0;
	unsigned char *pReadBuf = FlashReadBuff;
	unsigned char *pdata = (unsigned char *)pbuf;
	
	if(!pReadBuf || !pdata || !pbuf || length < 1)
	{
		return 1;
	}
	
	// 1 sector = 4KB = 4096B
	sector_number = (unsigned int)(write_addr / EX_FLASH_SECTOR_SIZE);
	sector_offset = (unsigned int)(write_addr % EX_FLASH_SECTOR_SIZE);
	sector_remain = EX_FLASH_SECTOR_SIZE - sector_offset;
	if(write_count <= sector_remain)
	{
		sector_remain = write_count;
	}
	
	while(1)
	{
		// 计算当前读写操作基地址
		wr_base_addrs = sector_number * EX_FLASH_SECTOR_SIZE;
		
		// 将本扇区的数据全部读出
		if(exflash_read(wr_base_addrs, pReadBuf, EX_FLASH_SECTOR_SIZE) != 0)
		{
			return 1;
		}
		
		for(i = sector_offset; i < EX_FLASH_SECTOR_SIZE; i++)
		{
			if(pReadBuf[i] != 0xFF)
			{
				break; // 需要擦除
			}
		}
		
		if(i < EX_FLASH_SECTOR_SIZE)
		{
			// 擦除扇区
			exflash_sector_erase(sector_number);
			
			// 将数据拷贝至扇区缓存
			for(i = 0; i < sector_remain; i++)
			{
				pReadBuf[i + sector_offset] = pdata[i];
			}
			
			// 将数据写入到 Flash 对应扇区内
			if(exflash_write_sector(wr_base_addrs, pReadBuf, EX_FLASH_SECTOR_SIZE) != 0)
			{
				return 1;
			}
		}
		else
		{
			// 已经擦除好了的空间，可直接将数据写入到 Flash 对应扇区内
			if(exflash_write_sector(write_addr, pdata, sector_remain) != 0)
			{
				return 1;
			}
		}
		
		if(sector_remain == write_count)
		{
			break; // 写入完毕
		}
		
		sector_number++; // 扇区号自增
		sector_offset = 0; // 新扇区偏移量为0
		
		pdata += sector_remain; // 数据指针偏移
		write_addr += sector_remain; // Flash 地址偏移
		write_count -= sector_remain; // 更新待写入字节总数
		sector_remain = (write_count > EX_FLASH_SECTOR_SIZE)? EX_FLASH_SECTOR_SIZE : (unsigned short)write_count;
	}
	
	return 0;
}


//=====================================================================================================
//                                      Flash Init Function
//=====================================================================================================


/********************************************************
 * 函数功能：读取外部 Flash 型号
 * 形    参：无
 * 返 回 值：外部 Flash 型号
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
eFlashType_t exflash_type_get(void)
{
	return eFlashType;
}

/********************************************************
 * 函数功能：读取外部 Flash 型号
 * 形    参：cs_pin：Flash片选引脚
 * 返 回 值：0=初始化成功
             1=未知Flash型号
             2=片选引脚初始化失败
 * 开 发 者：王志超
 * 维护日期：2020年5月25日
 * 修订日志：开发
 ********************************************************/
unsigned int exflash_init(unsigned int cs_pin)
{
	if(gpio_config(cs_pin, eGPIO_OUT_PP_UP, 1) == 0)
	{
		exFlashCsPin = cs_pin; // 必须先赋值
		eFlashType = exflash_read_device_id();
		if(eFlashType < W25Q80 || eFlashType > W25Q256)
		{
			printf("unknow Flash!\r\n");
			return 1; // 未知Flash型号
		}
	}
	else
	{
		printf("CSN_PIN init failed!\r\n");
		return 2; // 片选引脚初始化失败
	}
	printf("CSN_PIN init successfully!\r\n");
	return 0; // 初始化成功
}







