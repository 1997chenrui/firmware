#ifndef FLASH_H_
#define FLASH_H_

#include "bus.h"
#include <stdbool.h>
#include <stdint.h>

#define FLASH_MAGIC_ID 0xd8888888
#define FLASH_ADDRESS_MAGIC_ID 0xe8888888
#define FLASH_VERSION_SECTOR 3
#define FLASH_BOOT_START 0x08000000    // boot空间起始地址
#define FLASH_VERSION_START 0x0800C000 //版本信息空间起始地址
#define FLASH_APP_START 0x08010000     // app 应用空间起始地址
#define FLASH_APP_COPY 0x08060000      // app 备份空间起始地址

typedef struct {
    uint32_t magic[10];
    // 项目版本号，由上位机读写，与固件无关。
    uint32_t project_version;
    // Boot版本号
    uint32_t boot_version;
    // 电路板对应的地址
    uint32_t address;
} FirmWare_t;

void FLASH_If_Init(void);
uint32_t FLASH_If_Erase(int sectorIdx);
uint32_t FLASH_If_Write(uint32_t destination, uint8_t *p_source,
                        uint32_t length);
// 检查是否存在应用程序
bool Flash_ContainsApp(void);
// 检查是否存在应用程序
bool Flash_ContainsAddress(void);
// 设置项目版本号
void Flash_SetProjectVersion(uint32_t project_version);
// 获取项目版本号
uint32_t Flash_GetProjectVersion(void);
// 设置板卡地址
void Flash_SetAddressAndBootVersion(uint32_t address, uint32_t boot_version);
// 获取板卡地址
uint32_t Flash_GetAddress(void);
// 设置Boot版本号
void Flash_SetBootVersion(uint32_t boot_version);
// 获取Boot版本号
uint32_t Flash_GetBootVersion(void);

//------------ 固件相关 -----------------------------------------------
// 固件执行
void FirmWare_Excute(int cmd, bus_packet *packet);
// 跳转到App
void FirmWare_JumpToApp(void);
// 上报固件擦除完成事件
void Firmware_ReportEvent(void);
// 跳转到boot
void FirmWare_JumpToBoot(void);
#endif
