# JY-DT-YJ1F02_RTT_Nano

#### 介绍

圆形-丙烷，基础款家用可燃气体探测器_JY-DT-YJ1F02，基于RTT Nano 3.1.5

## RTT移植注意事项

> 参考WCH官方的文档[RISC-V 平台移植 RTOS](https://gitee.com/SXDT-Embedded/JY-DT-YJ1F02_RTT_Nano/blob/main/doc/RTOS%E7%A7%BB%E6%A4%8D%E5%88%B0RISC-V.pdf)

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

## 高精度延时

> 参考：
>
> [RTT文档-高精度延时](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/timer/timer?id=高精度延时)

## CPU利用率

> 参考：
>
> [【野火】RT-Thread内核实现与应用开发实战--基于STM32-16. CPU 利用率统计](https://doc.embedfire.com/rtos/rtthread/zh/latest/application/cpu_usage_rate.html#cpu)
>
> [RT-Thread CPU 利用率统计（学习笔记）](https://blog.csdn.net/weixin_43772810/article/details/125428073)
>
> [cpu_usage](https://github.com/enkiller/cpu_usage)

## 线程优先级

`drv_uart.c`中，串口使用DMA+空闲中断接收数据，在 `uart2_rx_dma_thread_entry` 线程中执行 `_UART2_RxCheck()`，其他想获取数据的线程优先级需**要低于**对应串口接收数据线程的优先级，以保证 `uart2_rx_dma_thread_entry`线程接收完所有数据后，其他线程才能通过获取 `uart2_rx_ok_sem`信号量来做数据处理。

## button

在 `bsp_button.h`中使用宏定义 `BUTTON_USE_THREAD`的定义来选择按键的扫描检测使用新建线程还是使用定时器，这里我经过对比还是用了定时器来做的

```c
// 不然就是用 timer 实现按键扫描处理
// #define BUTTON_USE_THREAD
```

### 使用一个线程做按键处理

![image-20230804155446677](image/image-20230804155446677.png)

![image-20230804155457152](image/image-20230804155457152.png)

### 使用一个定时器来做按键处理

![image-20230804155548242](image/image-20230804155548242.png)

![image-20230804155555526](image/image-20230804155555526.png)
