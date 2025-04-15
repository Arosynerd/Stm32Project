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

// 开启写使能
void W25Q32_WriteEnable(void)
{
    SPI1_CS_LOW();;
    SPI_SwapByte(0x06);
   SPI1_CS_HIGH();
}

// 关闭写使能
void W25Q32_WriteDisable(void)
{
    SPI1_CS_LOW();;
    SPI_SwapByte(0x04);
   SPI1_CS_HIGH();
}

// 等待状态不为忙（busy）
void W25Q32_WaitNotBusy(void)
{
    SPI1_CS_LOW();;

    // 发送读取状态寄存器指令
    SPI_SwapByte(0x05);

    // 等待收到的数据末位变成0
    while (SPI_SwapByte(0xff) & 0x01)
    {
    }

   SPI1_CS_HIGH();
}

// 擦除段（sector erase）,地址只需要块号和段号
void W25Q32_EraseSector(uint8_t block, uint8_t sector)
{
    // 首先等待状态不为忙
    W25Q32_WaitNotBusy();

    // 开启写使能
    W25Q32_WriteEnable();

    // 计算要发送的地址（段首地址）
    uint32_t addr = (block << 16) + (sector << 12);

    SPI1_CS_LOW();;

    // 发送指令
    SPI_SwapByte(0x20);

    SPI_SwapByte(addr >> 16 & 0xff); // 第一个字节
    SPI_SwapByte(addr >> 8 & 0xff);  // 第二个字节
    SPI_SwapByte(addr >> 0 & 0xff);  // 第三个字节

   SPI1_CS_HIGH();

    W25Q32_WriteDisable();
}

// 写入（页写）
void W25Q32_PageWrite(uint8_t block, uint8_t sector, uint8_t page, uint8_t innerAddr, uint8_t *data, uint16_t len)
{
  // 首先等待状态不为忙
  W25Q32_WaitNotBusy();

  // 开启写使能
  W25Q32_WriteEnable();

  // 计算要发送的地址（页首地址）
  uint32_t addr = (block << 16) + (sector << 12) + (page << 8) + innerAddr;

  SPI1_CS_LOW();

  // 发送指令
  SPI_SwapByte(0x02);

  // 发送24位地址
  SPI_SwapByte(addr >> 16); // 第一个字节
  SPI_SwapByte(addr >> 8);  // 第二个字节
  SPI_SwapByte(addr >> 0);  // 第三个字节

  //  依次发送数据
  for (uint16_t i = 0; i < len; i++)
  {
    SPI_SwapByte(data[i]);
  }

  SPI1_CS_HIGH();

  W25Q32_WriteDisable();
}

// 读取
void W25Q32_Read(uint8_t block, uint8_t sector, uint8_t page, uint8_t innerAddr, uint8_t *buffer, uint16_t len)
{
    // 首先等待状态不为忙
    W25Q32_WaitNotBusy();

    // 计算要发送的地址
    uint32_t addr = (block << 16) + (sector << 12) + (page << 8) + innerAddr;

    SPI1_CS_LOW();;

    // 发送指令
    SPI_SwapByte(0x03);

    // 发送24位地址
    SPI_SwapByte(addr >> 16 & 0xff); // 第一个字节
    SPI_SwapByte(addr >> 8 & 0xff);  // 第二个字节
    SPI_SwapByte(addr >> 0 & 0xff);  // 第三个字节

    //  依次读取数据
    for (uint16_t i = 0; i < len; i++)
    {
        buffer[i] = SPI_SwapByte(0xff);
    }

   SPI1_CS_HIGH();
}

