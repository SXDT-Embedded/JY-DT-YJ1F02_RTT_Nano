/*
 * @Author       : yzy
 * @Date         : 2023-04-04 14:45:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-07 17:03:49
 * @FilePath     : \JT-DT-YD1F01_RTT_Nano\bsp\src\bsp_led.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_led.h"
#include <rtthread.h>

#define LOG_TAG     "bsp_led"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

// ALIGN(RT_ALIGN_SIZE)
// static char led_thread_stack[LED_THREAD_STACK_SIZE];
// static struct rt_thread led_thread;

agile_led_t *led_r = RT_NULL;
agile_led_t *led_g = RT_NULL;
agile_led_t *led_y = RT_NULL;

agile_led_t *netled_r = RT_NULL;
agile_led_t *netled_g = RT_NULL;
agile_led_t *netled_y = RT_NULL;

// static void led_thread_entry(void *param)
// {
//     while (1)
//     {
//         rt_thread_mdelay(10);
//     }
// }

int BSP_LED_Init(void)
{
    led_r = agile_led_create(LED_R_PIN, PIN_HIGH, "100,200", 0);
    led_g = agile_led_create(LED_G_PIN, PIN_HIGH, "100,200", 0);
    led_y = agile_led_create(LED_Y_PIN, PIN_HIGH, "100,200", 0);

    netled_r = agile_led_create(NETLED_R_PIN, PIN_HIGH, "100,200", 0);
    netled_g = agile_led_create(NETLED_G_PIN, PIN_HIGH, "200,800", 0);
    netled_y = agile_led_create(NETLED_Y_PIN, PIN_HIGH, "100,200", 0);

    agile_led_start(led_r);
    agile_led_start(led_g);
    agile_led_start(led_y);

    // rt_thread_init(&led_thread,
    //                "led_thread",
    //                led_thread_entry,
    //                RT_NULL,
    //                &led_thread_stack[0],
    //                sizeof(led_thread_stack),
    //                LED_THREAD_PRIORITY, LED_THREAD_TIMESLICE);
    // rt_thread_startup(&led_thread);

    LOG_I("BSP_LED_Init");

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_APP_EXPORT(BSP_LED_Init);
#endif

static int led_test(void)
{
    agile_led_start(led_r);
    agile_led_start(led_g);
    agile_led_start(led_y);

    rt_thread_mdelay(5000);

    agile_led_stop(led_r);

    return 0;
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(led_test, led_test);
#endif // !FINSH_USING_MSH
