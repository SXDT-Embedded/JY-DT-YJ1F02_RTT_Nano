/*
 * @Author       : yzy
 * @Date         : 2023-04-04 14:45:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-07-25 17:14:39
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_led.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_led.h"
#include <rtthread.h>

#define DBG_TAG     "led"
#define DBG_LVL     DBG_LOG
#include <rtdbg.h>

int BSP_LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    LED_R_GPIO_CLK_ENABLE;
    LED_G_GPIO_CLK_ENABLE;
    LED_Y_GPIO_CLK_ENABLE;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = LED_R_PIN;
    GPIO_Init(LED_R_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_G_PIN;
    GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_Y_PIN;
    GPIO_Init(LED_Y_GPIO_PORT, &GPIO_InitStructure);

    LOG_D("USER_LED_Init!");
    return 0;
}
INIT_DEVICE_EXPORT(BSP_LED_Init);



