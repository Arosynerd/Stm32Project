# DMA

ROM向RAM传输10字节，完成后中断，主循环检测标志输出接收到的数据

## DMA配置

### 设置MEM2MEM模式

### 传输方向

### 地址自增

### 传输数据宽度

## DMA启动

### 使能DMA和中断

```
HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)&srcBuffer, (uint32_t)&dstBuffer, 10);
```

### 重写传送完成回调函数

```
//需要先重写再开启dma，不然赶不上
HAL_DMA_RegisterCallback(&hdma_memtomem_dma1_channel1, HAL_DMA_XFER_CPLT_CB_ID, DMA_Transmit_Complete_Callback);
```

注册回调函数

实现回调函数

## 主逻辑

```
 while (1)
  {
    /* USER CODE END WHILE */
    if(isFinish){
      for(int i = 0; i < 10; i++){
        printf("%d ", dstBuffer[i]); 
      }
      isFinish = 0;
    }
    /* USER CODE BEGIN 3 */
  }
```

 

