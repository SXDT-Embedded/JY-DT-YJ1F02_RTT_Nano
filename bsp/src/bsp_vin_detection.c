/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 17:51:08
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 17:53:23
 * @FilePath     : \JY-DT-YJ1F02_RTT_Nano\bsp\src\bsp_vin_detection.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_vin_detection.h"
#include "bsp_adc.h"
#include "bsp_rtc.h"
#include "bsp_flash.h"
#include "bsp_sys.h"

#define LOG_TAG     "bsp_vin_detection"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

void ADC1_2_IRQHandler(void) __attribute__((interrupt()));

int BSP_VIN_Detection_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    /* Higher Threshold: 14V, Lower Threshold: 10V */
    ADC_AnalogWatchdogThresholdsConfig(USED_ADC, VIN_ADC_HIGH_THRESHOLD, VIN_ADC_LOW_THRESHOLD);
    ADC_AnalogWatchdogSingleChannelConfig(USED_ADC, VIN_ADC_CHANNEL);
    ADC_AnalogWatchdogCmd(USED_ADC, ADC_AnalogWatchdog_SingleRegEnable);

    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ADC_ITConfig(USED_ADC, ADC_IT_AWD, ENABLE);
    LOG_D("BSP_VIN_Detection_Init");
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_BOARD_EXPORT(BSP_VIN_Detection_Init);
#endif

float Get_VIN_Voltage(void)
{
    float vin_voltage = Get_ADC_Voltage(kVinAdc) * VIN_VOLTAGE_RATIO;

    int16_t voltage = vin_voltage * 1000;

    LOG_D("Get_VIN_Voltage(V): %d.%d%d%d\r\n", (voltage / 1000), ((voltage % 1000) / 100),
        ((voltage % 100) / 10), (voltage % 10));

    return vin_voltage;
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(Get_VIN_Voltage, "Get_VIN_Voltage(V)");
#endif // !FINSH_USING_MSH

uint16_t Get_VIN_VoltageInt1000x(void)
{
    uint16_t voltage = (Get_ADC_Average(kVinAdc) * 3.3 / 4096) * VIN_VOLTAGE_RATIO * 1000;

    // LOG_D("Get_VIN_VoltageInt1000x = %04d", voltage);
    return voltage;
}

static void VIN_Detection_Process(void)
{
    LOG_D("Enter AnalogWatchdog Interrupt");
//    SysFlag.power_failure = SET;
    SysControl.status = kSysPowerDownStatus;
}

/*********************************************************************
 * @fn      ADC1_2_IRQHandler
 *
 * @brief   This function handles analog wathdog exception.
 *
 * @return  none
 */
void ADC1_2_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

	if(ADC_GetITStatus(ADC1, ADC_IT_AWD))
	{
        ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);

        ADC_ITConfig(USED_ADC, ADC_IT_AWD, DISABLE);

        VIN_Detection_Process();
        // TODO: 掉电执行的信号量
    }

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}



