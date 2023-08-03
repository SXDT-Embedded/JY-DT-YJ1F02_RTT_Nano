/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-07-27 12:56:09
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-03 14:14:33
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_buzzer.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */

#include "bsp_buzzer.h"
#include <rtthread.h>
#include <rthw.h>

//用到了atoi
#include <stdlib.h>

#define LOG_TAG     "bsp_buzzer"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

/**
 * @description: 蜂鸣器端口初始化
 * @return {*}
 */
int BSP_BUZZER_Init(void)
{
    rt_pin_mode(BUZZER_PIN, PIN_MODE_OUTPUT);
    BUZZER_OFF;

    LOG_I("BSP_BUZZER_Init");
    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_DEVICE_EXPORT(BSP_BUZZER_Init);
#endif

#if 0
static void test_us(int argc, char **argv)
{
    if (argc == 2)
    {
        uint32_t us = atoi(argv[1]);
        LOG_D("us = %d", us);

        BUZZER_ON;
        rt_hw_us_delay(us);
        BUZZER_OFF;
    }
    else
    {
        LOG_E("--used test_us [nus]");
        return;
    }
}
MSH_CMD_EXPORT(test_us, test_us(nus));
#endif

