#include "esp8266.h"
#include <string.h>


// 函数声明
void esp_cspin_init(void);
void esp_cspin_deinit(void);
_Bool resuleDetect(uint8_t index);

// 指令定义
const static char sendstaff[][112] = {
    "AT\r\n",
    "AT+CWMODE=1\r\n",
    "AT+CWJAP=\"huawei123\",\"111222333\"\r\n",
    "AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n",
    "AT+CIPMODE=1\r\n",
    "AT+CIPSEND\r\n",
    "GET https://api.seniverse.com/v3/weather/now.json?key=SOf79cS5d9w_Mx-3u&location=huizhou&language=en&unit=c\r\n"};

    
// 回复定义    
const static char reply[][30] = {
    "OK",
    "OK",
    "WIFI GOT IP",
    "CONNECT",
    "OK",
    "OK",
    "result"};

// 缓冲区定义
uint8_t datarecBuf[MAXBUF];

/**
 * @brief  根据是否进行 WiFi 配置发送一系列 AT 指令到 ESP8266 模块
 * 
 * @param IsWificonfig 布尔类型参数，当前函数未使用该参数，可根据后续需求用于控制是否进行 WiFi 配置
 * 
 * 该函数会先对 ESP8266 模块进行硬件复位，然后依次发送预定义的 AT 指令，
 * 并检查每个指令的回复是否符合预期。如果某个指令的回复不符合预期，
 * 则输出错误信息并停止发送后续指令。
 */
void espSendDatas(_Bool IsWificonfig)
{
    espHardres();
    HAL_Delay(1500);
    for (uint16_t i = 0; i < sizeof(sendstaff) / sizeof(sendstaff[0]); i++)
    {
        printf("===== %d times", i); // test

        espSendData((uint8_t *)sendstaff[i]);
        if (resuleDetect(i))
            continue;
        else
        {
            printf("send failed in step %d\n", i);
            break;
        }
    }
    printf("send done\n");
}
/**
 * @brief  通过串口2向外部设备发送数据
 * 
 * @param data 指向要发送的数据的指针，数据类型为无符号8位整数数组
 * 
 * 该函数使用HAL库的UART发送函数，将指定的数据通过串口2发送出去。
 * 发送的长度由传入数据的字符串长度决定，使用strlen函数计算。
 * 发送过程会一直阻塞，直到数据发送完成。
 */
void espSendData(uint8_t *data)
{
    HAL_UART_Transmit(&huart2, data, strlen((const char *)data), HAL_MAX_DELAY);
}

/**
 * @brief  对ESP8266模块进行硬件复位操作
 * 
 * 该函数后续应该会实现一系列操作，包括初始化相关引脚、拉低复位引脚、延时、
 * 拉高复位引脚、再次延时，最后反初始化相关引脚等步骤，以完成ESP8266的硬件复位。
 */
void espHardres(void)
{
    esp_cspin_init();
    RST_DOWN;
    HAL_Delay(100);
    RST_UP;
    HAL_Delay(100);
    esp_cspin_deinit();//复位完要悬空引脚，不然会出错
}



//本模块方法
_Bool resuleDetect(uint8_t index)
{
    uint8_t cnt = 5;
    _Bool res = 0;
    while (cnt--)
    {
        HAL_UART_Receive(&huart2, datarecBuf, MAXBUF, 1500);
        printf("count = %d,%s\n", cnt, datarecBuf);
        
        if (strstr((const char *)datarecBuf, reply[index]) != NULL)
        {
            res = 1;
            memset(datarecBuf, 0, MAXBUF);
            break;
        }
        else
            memset(datarecBuf, 0, MAXBUF);
    }
    return res;
}

void esp_cspin_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(ESPCS_GPIO_Port, ESPCS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin : ESPCS_Pin */
    GPIO_InitStruct.Pin = ESPCS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ESPCS_GPIO_Port, &GPIO_InitStruct);
}
void esp_cspin_deinit(void)
{
    HAL_GPIO_DeInit(ESPCS_GPIO_Port, ESPCS_Pin);
}
