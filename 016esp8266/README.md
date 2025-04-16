# README

esp8266.c

esp8266.h

这两个文件包含初始化及使用的所有api

## 函数结构

```c
void espSendData(uint8_t *data); //发送数据
void espSendDatas(_Bool IsWificonfig);//发送接收数据集并判断数据
void espHardres(void);//硬复位
```



## 函数解释

```c
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
```

连续发送数据指令集，每条指令都有对应的返回判断，失败则退出

```c
void espHardres(void)
{
    esp_cspin_init();
    RST_DOWN;
    HAL_Delay(100);
    RST_UP;
    HAL_Delay(100);
    esp_cspin_deinit();//复位完要悬空引脚，不然会出错
}
```

复位完要悬空引脚，不然会出错
