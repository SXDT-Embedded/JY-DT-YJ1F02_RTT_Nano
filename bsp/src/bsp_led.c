/*
 * @Author       : yzy
 * @Date         : 2023-04-04 14:45:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-03 16:15:23
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

#include "easyblink.h"

ebled_t led1 = RT_NULL;
ebled_t led2 = RT_NULL;
ebled_t led3 = RT_NULL;

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

    LOG_I("BSP_LED_Init");

    led1 = easyblink_init_led(NETLED_R_PIN, PIN_HIGH);
    led2 = easyblink_init_led(NETLED_G_PIN, PIN_HIGH);
    led3 = easyblink_init_led(NETLED_Y_PIN, PIN_HIGH);

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_APP_EXPORT(BSP_LED_Init);
#endif

static int led_test(void)
{
    easyblink_stop(led1);
    easyblink_stop(led2);
    easyblink_stop(led3);

    // rt_thread_mdelay(5000);
    easyblink(led1, 10, 1000, 200);
    easyblink(led2, 10, 1000, 200);
    easyblink(led3, 10, 1000, 200);

    return 0;
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(led_test, led_test);
#endif // !FINSH_USING_MSH
