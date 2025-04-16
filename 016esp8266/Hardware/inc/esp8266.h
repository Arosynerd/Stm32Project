#ifndef __ESP8266_H
#define __ESP8266_H

#include <stdint.h>
#include "usart.h"

#define MAXBUF 275
#define RST_DOWN HAL_GPIO_WritePin(ESPCS_GPIO_Port, ESPCS_Pin, GPIO_PIN_SET)
#define RST_UP HAL_GPIO_WritePin(ESPCS_GPIO_Port, ESPCS_Pin, GPIO_PIN_RESET)


void espSendData(uint8_t *data); 
void espSendDatas(_Bool IsWificonfig);
void espHardres(void);
#endif /* __ESP8266_H */
