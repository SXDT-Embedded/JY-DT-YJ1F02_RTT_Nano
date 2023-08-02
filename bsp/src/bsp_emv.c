/*
 * @Author       : yzy
 * @Date         : 2023-01-31 12:20:20
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 16:36:42
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_emv.c
 * @Description  : 电磁阀 EMV
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_emv.h"
#include "bsp_adc.h"

#define LOG_TAG     "bsp_emv"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

// 用到了atoi
// #include <stdlib.h>

// ALIGN(RT_ALIGN_SIZE)
// static char coil_thread_stack[512];
// static struct rt_thread coil_thread;

void Coil_Run(TeCoilDirection direction)
{
    switch (direction)
    {
    case kCoast:
        EMV_A_LOW;
        EMV_B_LOW;
        break;
    case kForward:
        EMV_A_HIGH;
        EMV_B_LOW;
        break;
    case kBackward:
        EMV_A_LOW;
        EMV_B_HIGH;
        break;
    case kBrake:
        EMV_A_HIGH;
        EMV_B_HIGH;
        break;

    default:
        break;
    }
}

float Get_Coil_Current(void)
{
    float coil_current_ma = Get_ADC_Voltage(kCoilAdc) / EMV_CSR * 1000;

    uint16_t ma = coil_current_ma * 1000;

    LOG_D("coil_current_ma: %d.%d%d%d", (ma / 1000), ((ma % 1000) / 100),
        ((ma % 100) / 10), (ma % 10));

    return coil_current_ma;
}

int Coil_Close(void)
{
    Coil_Run(kForward);
    rt_thread_mdelay(EMV_ACTION_MS);
    Coil_Run(kCoast);

    LOG_D("Coil_Close");

    return 0;
}
MSH_CMD_EXPORT(Coil_Close, "Coil_Close");

int BSP_Coil_Init(void)
{
    rt_pin_mode(EMV_A_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(EMV_B_PIN, PIN_MODE_OUTPUT);

    EMV_A_LOW;
    EMV_B_LOW;

    LOG_I("BSP_Coil_Init");

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_BOARD_EXPORT(BSP_Coil_Init);
#endif

