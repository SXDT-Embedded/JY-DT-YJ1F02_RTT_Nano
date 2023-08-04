# JT-DT-YD1F01_RTT_Nano

#### 介绍

基础款家用可燃气体探测器（甲烷）JT-DT-YD1F01 裸机框架移植到RTT Nano 3.1.5

## RTT移植注意事项

> 参考WCH官方的文档[RISC-V 平台移植 RTOS](https://gitee.com/SXDT-Embedded/JT-DT-YD1F01_RTT_Nano/blob/main/doc/RTOS%E7%A7%BB%E6%A4%8D%E5%88%B0RISC-V.pdf)

### 中断

这里对于CH32V203C8T6在 MRS 声明中断函数时由 `__attribute__((interrupt()))`方式定义编译器添加软件出入栈的代码。

比如在 `drv_usart.c`中，

```c
#ifdef BSP_USING_UART1
void USART1_IRQHandler(void) __attribute__((interrupt()));
void USART1_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART1_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif
```

### drv.usart.c

这里我复制的是在RTT Studio基于CH32V307的BSP生成的4.1.1版本的代码中的驱动文件 `drv.usart.c`的内容

要注意的是CH32V203C8T6只有**4个串口**，只保留其中USART1/2/3，UART4，其余删掉


## 线程优先级

`drv_uart.c`中，串口使用DMA+空闲中断接收数据，在 `uart2_rx_dma_thread_entry` 线程中执行 `_UART2_RxCheck()`，其他想获取数据的线程优先级需**要低于**对应串口接收数据线程的优先级，以保证 `uart2_rx_dma_thread_entry`线程接收完所有数据后，其他线程才能通过获取 `uart2_rx_ok_sem`信号量来做数据处理。
