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

#include "drv_uart.h"

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
// 写到 drv_uart.c了
/* 移植控制台，实现控制台输出, 对接 rt_hw_console_output */
// RT_WEAK void rt_hw_console_output(const char *str)
// RT_WEAK char rt_hw_console_getchar(void)

#endif //! RT_USING_SERIAL


#endif /* RT_USING_FINSH */

