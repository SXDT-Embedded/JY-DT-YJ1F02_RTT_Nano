/*
 * @Author       : yzy
 * @Date         : 2023-04-04 14:45:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-03 15:34:33
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_led.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_led.h"
#include <rtthread.h>

#define LOG_TAG     "bsp_led"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

#include "littled.h"

static int led1, led2, led3;

int BSP_LED_Init(void)
{
    rt_pin_mode(LED_R_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_G_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_Y_PIN, PIN_MODE_OUTPUT);

    rt_pin_mode(NETLED_R_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(NETLED_G_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(NETLED_Y_PIN, PIN_MODE_OUTPUT);

    LED_R_OFF;
    LED_G_OFF;
    LED_Y_OFF;

    NETLED_R_OFF;
    NETLED_G_OFF;
    NETLED_Y_OFF;

    led1 = led_register(NETLED_R_PIN, PIN_HIGH);
    led2 = led_register(NETLED_G_PIN, PIN_HIGH);
    led3 = led_register(NETLED_Y_PIN, PIN_HIGH);

    LOG_I("BSP_LED_Init");

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_APP_EXPORT(BSP_LED_Init);
#endif

static int littled_sample(void)
{
    LED_ON(led1);
    rt_thread_mdelay(1000);
    LED_OFF(led1);
    rt_thread_mdelay(1000);

    LED_ON(led2);
    rt_thread_mdelay(1000);
    LED_OFF(led2);
    rt_thread_mdelay(1000);

    LED_ON(led3);
    rt_thread_mdelay(1000);
    LED_OFF(led3);
    rt_thread_mdelay(1000);

    // led_unregister(led_test);

    return 0;
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(littled_sample, Driver LED based on littled);
#endif
