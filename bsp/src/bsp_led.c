/*
 * @Author       : yzy
 * @Date         : 2023-04-04 14:45:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-03 15:55:06
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

static int ebled_test(void)
{
    /* led1 闪3次，周期1000ms，亮500ms */
    easyblink(led1, 3, 500, 1000);
    /* led1 上次的闪完后，再接着闪2次，周期2000ms，亮1500ms */
    easyblink(led1, 2, 1500, 2000);
    /* led2 一直闪，周期1000ms，亮5ms */
    easyblink(led2, -1, 5, 1000);

    rt_thread_mdelay(8000);

    /* 打断led2闪烁，开始闪5次，周期500ms，亮300ms，闪完后恢复led2前次的序列 */
    easyblink(led2, 5, 300, 500);

    easyblink(led1, -1, 10, 5000);

    rt_thread_mdelay(8000);

    /* 中途打断，停止闪烁 */
    easyblink_stop(led2);

    rt_thread_mdelay(5000);

    return 0;
}
#ifdef FINSH_USING_MSH
// MSH_CMD_EXPORT(ebled_test, ebled_test);
#endif // !FINSH_USING_MSH
