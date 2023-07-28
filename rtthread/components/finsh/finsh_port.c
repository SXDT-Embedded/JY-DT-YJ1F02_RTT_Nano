/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rthw.h>
#include <rtconfig.h>

#include "drv_usart.h"

#ifndef RT_USING_FINSH
#error Please uncomment the line <#include "finsh_config.h"> in the rtconfig.h
#endif

#ifdef RT_USING_FINSH

#if RT_USING_SERIAL
#include "serial.h"
char rt_hw_console_getchar(void)
{
    /* Note: the initial value of ch must < 0 */
    int ch = -1;

    struct rt_device *serial1;
    serial1 = rt_device_find("uart1");

//#error "TODO 4: Read a char from the uart and assign it to 'ch'."
    rt_device_read(serial1, -1, &ch, 1);

    return ch;
}
#else // RT_USING_SERIAL
// 写到 drv_usart.c了
/* 移植控制台，实现控制台输出, 对接 rt_hw_console_output */
RT_WEAK void rt_hw_console_output(const char *str)
{

    rt_size_t i = 0, size = 0, j = 0;
    char a = '\r';

    size = rt_strlen(str);
    rt_uint8_t buf[size * 2];

    for (i = 0; i < size; i++, j++)
    {
        if (*(str + i) == '\n')
        {
            // USART1_SendArray(&a, 1);
            buf[j] = a;
            j++;
        }
        // USART1_SendArray((str + i), 1);
        buf[j] = str[i];
    }

    USART1_SendArray(buf, j);
}

// extern rt_sem_t uart1_rx_check_sem;

/* 移植 FinSH，实现命令行交互, 需要添加 FinSH 源码，然后再对接 rt_hw_console_getchar */
/* 中断方式 */
RT_WEAK char rt_hw_console_getchar(void)
{
    char ch = 0;

    // /* 从 ringbuffer 中拿出数据 */
    // while (rt_ringbuffer_getchar(&uart_rxcb, (rt_uint8_t *)&ch) != 1)
    // {
    //     rt_sem_take(&shell_rx_sem, RT_WAITING_FOREVER);
    // }

    // while (lwrb_read(&usart1_rx_rb, &ch, 1) != 1)
    // {
    //     rt_sem_take(uart1_rx_check_sem, RT_WAITING_FOREVER);
    // }

    lwrb_read(&usart1_rx_rb, &ch, 1);

    return ch;
}
#endif //! RT_USING_SERIAL


#endif /* RT_USING_FINSH */

