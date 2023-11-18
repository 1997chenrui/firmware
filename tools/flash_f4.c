/**
 * @file flash_f4.c
 * @author wangz modify
 * @brief 提供功能如下：
 * 1、板卡地址：boot中写入板卡地址，在App中读取，将地址与App解耦。
 * 2、Boot版本号：写入flash中，以便App可以读取。
 * 3、项目版本号：由上位机读写，通过版本号确定flash中是否存在固件，确定该固件在项目中的版本号
 * 由于版本号是App写入的，所以存在固件就一定存在地址
 * @version 0.1
 * @date 2022-05-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "ErrCode.h"
#include "flash.h"
#include "stm32f4xx_hal.h"
#include <string.h>

#define FLASH_DATA_BASE_ADDR                                                   \
    (FLASH_BASE + (512 - 128) * 1024) //第6个扇区尾，第7个扇区始

#define FLASH_START_ADRESS FLASH_BASE // 0x0800 0000
#define FLASH_PAGE_NBPERBANK 12       //主存储器分为12个扇区
#define FLASH_BANK_NUMBER 1

enum {
    FLASHIF_OK = 0,
    FLASHIF_ERASEKO,
    FLASHIF_WRITINGCTRL_ERROR,
    FLASHIF_WRITING_ERROR,
    FLASHIF_PROTECTION_ERRROR
};

/* protection type */
enum {
    FLASHIF_PROTECTION_NONE = 0,
    FLASHIF_PROTECTION_PCROPENABLED = 0x1,
    FLASHIF_PROTECTION_WRPENABLED = 0x2,
    FLASHIF_PROTECTION_RDPENABLED = 0x4,
};

#define USER_FLASH_END_ADDRESS (FLASH_BASE + 512 * 1024) // 0-7 共8个扇区

//初始化,clear所有标志位
void FLASH_If_Init(void) {
    /* Unlock the Program memory */
    HAL_FLASH_Unlock();

    /* Clear all FLASH flags */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR |
                           FLASH_FLAG_WRPERR | FLASH_FLAG_OPERR);
    /* Unlock the Program memory */
    HAL_FLASH_Lock();
}

//擦除
uint32_t FLASH_If_Erase(int sectorIdx) {
    uint32_t PageError = 0;
    FLASH_EraseInitTypeDef pEraseInit;
    HAL_StatusTypeDef status = HAL_OK;
    HAL_FLASH_Unlock();
    pEraseInit.Banks =
        FLASH_BANK_1; // Select banks to erase when Mass erase is enabled.
    pEraseInit.NbSectors = 1;
    pEraseInit.Sector = sectorIdx;
    pEraseInit.TypeErase =
        FLASH_TYPEERASE_SECTORS; // FLASH_TYPEERASE_SECTORS：区擦除
                                 // FLASH_TYPEERASE_MASSERASE：批量擦除
    pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError); //擦除,成功返回 HAL_OK
    HAL_FLASH_Lock();
    if (status != HAL_OK) {
        return FLASHIF_ERASEKO;
    }
    return FLASHIF_OK;
}

//写
uint32_t FLASH_If_Write(uint32_t destination, uint8_t *p_source,
                        uint32_t length) {
    uint32_t status = FLASHIF_OK;
    HAL_FLASH_Unlock();
    for (int i = 0; i < length; i++) {
        if ((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, destination + i,
                                        p_source[i])) != HAL_OK) {
            printf("flash program err!!\r\n");
            break;
        }
    }
    HAL_FLASH_Lock();
    return status;
}

static uint32_t Flash_SetObject(void *obj, uint32_t len) {
    uint32_t base = FLASH_MAGIC_ID;
    FLASH_If_Init();
    uint32_t code = FLASH_If_Erase(FLASH_SECTOR_7);
    if (code == 0) {
        code = FLASH_If_Write(FLASH_DATA_BASE_ADDR, (uint8_t *)&base, 4);
        code = FLASH_If_Write(FLASH_DATA_BASE_ADDR + 4, obj, len);
    } else {
        code = ERR_FLASH_ERROR;
    }
    return code;
}

/**
 * @brief 检查flash中是否存在固件，存在版本号即认为存在固件
 *
 * @return true 存在
 * @return false 不存在
 */
bool Flash_ContainsApp(void) //检查是否存在应用程序
{
    FirmWare_t *flash = (FirmWare_t *)FLASH_VERSION_START;
    for (int i = 0; i < 10; i++) {
        if (flash->magic[i] != FLASH_MAGIC_ID) {
            return false;
        }
    }
    return true;
}

/**
 * @brief 检查flash中是否存在地址
 *
 * @return true 存在
 * @return false 不存在
 */
bool Flash_ContainsAddress(void) {
    FirmWare_t *flash = (FirmWare_t *)FLASH_VERSION_START;
    for (int i = 0; i < 10; i++) {
        if (flash->magic[i] != FLASH_ADDRESS_MAGIC_ID &&
            flash->magic[i] != FLASH_MAGIC_ID) {
            return false;
        }
    }
    return true;
}

/**
 * @brief 获取地址
 *
 * @return int 0 无效地址 其他值 地址
 */
uint32_t Flash_GetAddress(void) {
    FirmWare_t *flash = (FirmWare_t *)FLASH_VERSION_START;
    for (int i = 0; i < 10; i++) {
        if (flash->magic[i] != FLASH_ADDRESS_MAGIC_ID &&
            flash->magic[i] != FLASH_MAGIC_ID) {
            return 0;
        }
    }
    return flash->address;
}

/**
 * @brief 设置地址和Boot版本号
 *
 * @param address 地址
 *
 * @param boot_version Boot版本号
 */
void Flash_SetAddressAndBootVersion(uint32_t address, uint32_t boot_version) {
    FirmWare_t f;
    f.project_version = Flash_GetProjectVersion();
    f.address = address;
    f.boot_version = boot_version;
    for (int i = 0; i < 10; i++) {
        f.magic[i] = FLASH_ADDRESS_MAGIC_ID;
    }
    FLASH_If_Init();
    FLASH_If_Erase(FLASH_VERSION_SECTOR);
    FLASH_If_Write(FLASH_VERSION_START, (uint8_t *)&f, sizeof(f));
}

/**
 * @brief 获取Boot版本号
 *
 * @return int 0 无效版本号，其他值 版本号
 */
uint32_t Flash_GetBootVersion(void) {
    FirmWare_t *flash = (FirmWare_t *)FLASH_VERSION_START;
    for (int i = 0; i < 10; i++) {
        if (flash->magic[i] != FLASH_MAGIC_ID &&
            flash->magic[i] != FLASH_ADDRESS_MAGIC_ID) {
            return 0;
        }
    }
    return flash->project_version;
}

/**
 * @brief 获取项目版本号
 *
 * @return int 0 无效版本号，其他值 版本号
 */
uint32_t Flash_GetProjectVersion(void) {
    FirmWare_t *flash = (FirmWare_t *)FLASH_VERSION_START;
    for (int i = 0; i < 10; i++) {
        if (flash->magic[i] != FLASH_MAGIC_ID) {
            return 0;
        }
    }
    return flash->project_version;
}

/**
 * @brief 设置项目版本号
 *
 * @param project_version 版本号
 */
void Flash_SetProjectVersion(uint32_t project_version) {
    FirmWare_t f;
    f.address = Flash_GetAddress();
    f.boot_version = Flash_GetBootVersion();
    f.project_version = project_version;
    for (int i = 0; i < 10; i++) {
        f.magic[i] = FLASH_MAGIC_ID;
    }
    FLASH_If_Init();
    FLASH_If_Erase(FLASH_VERSION_SECTOR);
    FLASH_If_Write(FLASH_VERSION_START, (uint8_t *)&f, sizeof(f));
}

void FirmWare_JumpToApp(void) {
    //在0x8010000的位置存的是MSP的初始值，此值将放入MSP寄存器；0x8010004的位置放的是复位向量，放入PC寄存器中，然后从PC中存取的
    //地址处取指并开始执行,故+4
    uint32_t JumpAddress = *(__IO uint32_t *)(FLASH_APP_START + 4);
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t *)FLASH_APP_START);
    ((void (*)(void))JumpAddress)();
}

void FirmWare_JumpToBoot(void) {
    //在0x8010000的位置存的是MSP的初始值，此值将放入MSP寄存器；0x8010004的位置放的是复位向量，放入PC寄存器中，然后从PC中存取的
    //地址处取指并开始执行,故+4
    uint32_t JumpAddress = *(__IO uint32_t *)(FLASH_BOOT_START + 4);
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t *)FLASH_BOOT_START);
    ((void (*)(void))JumpAddress)();
}
