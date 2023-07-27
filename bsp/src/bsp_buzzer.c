/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-07-26 08:38:26
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-07-27 13:01:10
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_buzzer.c
 * @Description  :
 *
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved.
 */
#include "bsp_buzzer.h"
#include <rtthread.h>
#include <rthw.h>

//用到了atoi()
#include <stdlib.h>

#define DBG_TAG "buzzer"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/**
 * @description: 蜂鸣器端口初始化
 * @return {*}
 */
int BSP_BUZZER_Init(void)
{
    rt_pin_mode(BUZZER_PIN, PIN_MODE_OUTPUT);
    BUZZER_OFF();

    LOG_D("BSP_BUZZER_Init");
    return 0;
}
INIT_DEVICE_EXPORT(BSP_BUZZER_Init);

static void test_us(int argc, char **argv)
{
    if (argc == 2)
    {
        uint32_t us = atoi(argv[1]);
        LOG_D("us = %d", us);

        BUZZER_ON();
        rt_hw_us_delay(us);
        BUZZER_OFF();
    }
    else
    {
        LOG_E("--used test_us [nus]");
        return;
    }
}
MSH_CMD_EXPORT(test_us, test_us(nus));
