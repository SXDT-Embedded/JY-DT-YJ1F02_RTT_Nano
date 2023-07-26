/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-07-26 08:38:26
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-07-26 08:39:42
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_buzzer.c
 * @Description  :
 *
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved.
 */
#include "bsp_buzzer.h"
#include <rtthread.h>
#include <rthw.h>

#define DBG_TAG "buzzer"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/**
 * @description: 蜂鸣器端口初始化
 * @return {*}
 */
int BSP_BUZZER_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    BUZZER_GPIO_CLK_ENABLE;

    GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);

    BUZZER_OFF;

    LOG_D("USER_BUZZER_Init!");
    return 0;
}
INIT_DEVICE_EXPORT(BSP_BUZZER_Init);

static void test_us(void)
{
    BUZZER_ON;
    rt_hw_us_delay(900);
    BUZZER_OFF;
}
MSH_CMD_EXPORT(test_us, test_us);
