/*
 * @Author       : yzy
 * @Date         : 2023-04-04 14:45:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-03 16:40:43
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

#include "agile_led.h"

static agile_led_t *led1 = RT_NULL;
static agile_led_t *led2 = RT_NULL;
static agile_led_t *led3 = RT_NULL;

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

    led1 = agile_led_create(NETLED_R_PIN, PIN_HIGH, "100,200", -1);
    led2 = agile_led_create(NETLED_G_PIN, PIN_HIGH, "200,800", 10);
    led3 = agile_led_create(NETLED_Y_PIN, PIN_HIGH, "100,200", 3);

    LOG_I("BSP_LED_Init");

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_APP_EXPORT(BSP_LED_Init);
#endif

static int led_test(void)
{
    agile_led_start(led1);
    agile_led_start(led2);
    agile_led_start(led3);

    rt_thread_mdelay(5000);

    agile_led_stop(led1);

    return 0;
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(led_test, led_test);
#endif // !FINSH_USING_MSH
