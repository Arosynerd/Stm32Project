#ifndef W32QXX_H
#define W32QXX_H

#include "spi.h"

#define SPI1_CS_LOW() HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define SPI1_CS_HIGH() HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)

uint8_t SPI_SwapByte(uint8_t byte);
void readID(uint8_t *mid, uint16_t *did);
// 开启写使能
void W25Q32_WriteEnable(void);

// 关闭写使能
void W25Q32_WriteDisable(void);

// 等待状态不为忙（busy）
void W25Q32_WaitNotBusy(void);

// 擦除段（sector erase）,地址只需要块号和段号
void W25Q32_EraseSector(uint8_t block, uint8_t sector);

// 写入（页写）
void W25Q32_PageWrite(uint8_t block, uint8_t sector, uint8_t page, uint8_t innerAddr, uint8_t *data, uint16_t len);

// 读取
void W25Q32_Read(uint8_t block, uint8_t sector, uint8_t page, uint8_t innerAddr, uint8_t * buffer, uint16_t len);
#endif

