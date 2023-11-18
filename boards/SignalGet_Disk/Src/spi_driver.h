#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_
#include <stdint.h>



#define SPIDRIVER_SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPIDRIVER_SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)
#define SPIDRIVER_I2S_FLAG_CHSIDE                 ((uint16_t)0x0004)
#define SPIDRIVER_I2S_FLAG_UDR                    ((uint16_t)0x0008)
#define SPIDRIVER_SPI_FLAG_CRCERR                 ((uint16_t)0x0010)
#define SPIDRIVER_SPI_FLAG_MODF                   ((uint16_t)0x0020)
#define SPIDRIVER_SPI_I2S_FLAG_OVR                ((uint16_t)0x0040)
#define SPIDRIVER_SPI_I2S_FLAG_BSY                ((uint16_t)0x0080)
#define SPIDRIVER_SPI_I2S_FLAG_TIFRFE             ((uint16_t)0x0100)
typedef enum {SPIDRIVER_RESET = 0, SPIDRIVER_SET = !SPIDRIVER_RESET} SPIDRIVER_FlagStatus, SPIDRIVER_ITStatus;
typedef struct
{
  volatile uint16_t CR1;        /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
  uint16_t      RESERVED0;  /*!< Reserved, 0x02                                                           */
  volatile uint16_t CR2;        /*!< SPI control register 2,                             Address offset: 0x04 */
  uint16_t      RESERVED1;  /*!< Reserved, 0x06                                                           */
  volatile uint16_t SR;         /*!< SPI status register,                                Address offset: 0x08 */
  uint16_t      RESERVED2;  /*!< Reserved, 0x0A                                                           */
  volatile uint16_t DR;         /*!< SPI data register,                                  Address offset: 0x0C */
  uint16_t      RESERVED3;  /*!< Reserved, 0x0E                                                           */
  volatile uint16_t CRCPR;      /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  uint16_t      RESERVED4;  /*!< Reserved, 0x12                                                           */
  volatile uint16_t RXCRCR;     /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
  uint16_t      RESERVED5;  /*!< Reserved, 0x16                                                           */
  volatile uint16_t TXCRCR;     /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
  uint16_t      RESERVED6;  /*!< Reserved, 0x1A                                                           */
  volatile uint16_t I2SCFGR;    /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  uint16_t      RESERVED7;  /*!< Reserved, 0x1E                                                           */
  volatile uint16_t I2SPR;      /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
  uint16_t      RESERVED8;  /*!< Reserved, 0x22                                                           */
} SPIDRIVER_SPI_TypeDef;


#define SPIDRIVER_PERIPH_BASE           ((uint32_t)0x40000000) 
#define SPIDRIVER_APB1PERIPH_BASE       SPIDRIVER_PERIPH_BASE
#define SPIDRIVER_SPI2_BASE             (SPIDRIVER_APB1PERIPH_BASE + 0x3800)
#define SPIDRIVER_SPI2                  ((SPIDRIVER_SPI_TypeDef *) SPIDRIVER_SPI2_BASE)

void SPIDRIVER_AD7699SPI2Config(void);
SPIDRIVER_FlagStatus SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG);
void SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI_TypeDef* SPIx, uint16_t Data);
uint16_t SPIDRIVER_SPI_I2S_ReceiveData(SPIDRIVER_SPI_TypeDef* SPIx);

#endif
