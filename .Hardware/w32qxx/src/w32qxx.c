#include "w32qxx.h"

/*

using spi 2
CS pin pd12

*/
uint8_t SPI_SwapByte(uint8_t byte)
{
  uint8_t result;
  HAL_SPI_TransmitReceive(&hspi2, &byte, &result, 1, 1000);
  return result;
}
void readID(uint8_t *mid, uint16_t *did)
{
  SPI1_CS_LOW();
  SPI_SwapByte(0x9F);
  *mid = SPI_SwapByte(0x9f);
  *did = 0;
  *did |= SPI_SwapByte(0xff) << 8;
  *did |= SPI_SwapByte(0xff);
  SPI1_CS_HIGH();
}


