#ifndef W32QXX_H
#define W32QXX_H

#include "spi.h"

#define SPI1_CS_LOW() HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define SPI1_CS_HIGH() HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)

uint8_t SPI_SwapByte(uint8_t byte);
void readID(uint8_t *mid, uint16_t *did);
#endif

