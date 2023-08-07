/*
 * @Author       : yzy
 * @Date         : 2023-01-30 12:50:12
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-07 11:17:57
 * @FilePath     : \JT-DT-YD1F01_RTT_Nano\bsp\src\bsp_adc.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_adc.h"

#define LOG_TAG     "bsp_adc"       // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

//用到了atoi
#include <stdlib.h>

uint16_t adc_buffer[ADC_BUFFER_SIZE];
static int16_t adc_calibrattion;

/**
 * @description: Get Conversion Value.
 * @param {int16_t} val     Sampling value
 * @return {*}  val+Calibrattion_Val - Conversion Value.
 */
static uint16_t _Get_ConversionVal(int16_t val)
{
    if((val + adc_calibrattion) < 0)
        return 0;
    if((adc_calibrattion + val) > 4095 || val == 4095)
        return 4095;
    return (val + adc_calibrattion);
}

/**
 * @description: ADC管脚GPIO初始化GPIO_Mode_AIN
 * @return {*}
 */
static void _USED_ADC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIO 时钟 GPIOA
    USED_ADC_GPIO_CLK_ENABLE;

    // 配置 IO
    GPIO_InitStructure.GPIO_Pin = EMV_ADC_GPIO_PIN | MQ_ADC_GPIO_PIN | VIN_ADC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(USED_ADC_GPIO_PORT, &GPIO_InitStructure);
}

static void _USED_ADC_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    USED_ADC_DMA_CLK_ENABLE;

    DMA_DeInit(USED_ADC_DMA_STREAM);
    DMA_InitStructure.DMA_PeripheralBaseAddr = USED_ADC_DR_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(USED_ADC_DMA_STREAM, &DMA_InitStructure);
}

int BSP_ADC_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    // NVIC_InitTypeDef NVIC_InitStructure;

    _USED_ADC_GPIO_Init();
    _USED_ADC_DMA_Init();

    // 开启ADC时钟
    USED_ADC_CLK_ENABLE;
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    ADC_DeInit(USED_ADC);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;        // 使能扫描
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 禁止外部边沿触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = NUM_OF_CHANNEL;    // 转换通道 (3) 个
    ADC_Init(USED_ADC, &ADC_InitStructure);

    // 设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    ADC_RegularChannelConfig(USED_ADC, EMV_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(USED_ADC, MQ_ADC_CHANNEL, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(USED_ADC, VIN_ADC_CHANNEL, 3, ADC_SampleTime_239Cycles5);

    // /* Higher Threshold: 14V, Lower Threshold: 10V */
    // ADC_AnalogWatchdogThresholdsConfig(USED_ADC, VIN_ADC_HIGH_THRESHOLD, VIN_ADC_LOW_THRESHOLD);
    // ADC_AnalogWatchdogSingleChannelConfig(USED_ADC, VIN_ADC_CHANNEL);
    // ADC_AnalogWatchdogCmd(USED_ADC, ADC_AnalogWatchdog_SingleRegEnable);

    // NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);

    // ADC_ITConfig(USED_ADC, ADC_IT_AWD, ENABLE);

    ADC_DMACmd(USED_ADC, ENABLE);
    ADC_Cmd(USED_ADC, ENABLE);

    ADC_BufferCmd(USED_ADC, DISABLE); //disable buffer
    ADC_ResetCalibration(USED_ADC);
    while(ADC_GetResetCalibrationStatus(USED_ADC));
    ADC_StartCalibration(USED_ADC);
    while(ADC_GetCalibrationStatus(USED_ADC));
    adc_calibrattion = Get_CalibrationValue(USED_ADC);

    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    LOG_D("adc_calibrattion = %d", adc_calibrattion);
    LOG_I("BSP_ADC_Init");

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_BOARD_EXPORT(BSP_ADC_Init);
#endif

/**
 * @description: ADC平均值计算
 * @param {TeAdcIndex} index
 * @return {*}
 */
float Get_ADC_Average(TeAdcIndex index)
{
    uint32_t sum = 0;
    float average = 0;

    for (uint8_t i = index; i < ADC_BUFFER_SIZE; i += NUM_OF_CHANNEL)
    {
        sum += _Get_ConversionVal(adc_buffer[i]);
    }
    average = sum / ADC_TIMES;

    return average;
}

// 这里测的是原始的ADC电压，没有经过缩放
float Get_ADC_Voltage(TeAdcIndex index)
{
    float voltage = Get_ADC_Average(index) * 3.3 / 4096;

    // LOG_D("Get_ADC_Voltage(%d) = %d", index, (int)(voltage * 1000));
    LOG_D("Get_ADC(%d)_Voltage(V): %d.%d%d%d", index
                    , (int)voltage, (int)(voltage * 10) % 10, (int)(voltage * 100) % 10, (int)(voltage * 1000) % 10);

    return voltage;
}

uint16_t Get_ADC_VoltageInt1000x(TeAdcIndex index)
{
    uint16_t voltage = (Get_ADC_Average(index) * 3.3 / 4096) * 1000;

    LOG_D("VoltageInt1000x(%d) = %04d", index, voltage);

    return voltage;
}

// 这里测的是原始的ADC电压，没有经过缩放
static int TEST_Get_ADC_Voltage(int argc, char **argv)
{
    if (argc == 2)
    {
        TeAdcIndex index = atoi(argv[1]);
        if (0 <= index && index <= 2)
        {
            float voltage = Get_ADC_Average(index) * 3.3 / 4096;

            LOG_D("Get_ADC(%d)_Voltage(V): %d.%d%d%d", index
                    , (int)voltage, (int)(voltage * 10) % 10, (int)(voltage * 100) % 10, (int)(voltage * 1000) % 10);
        }
        else
        {
            goto err;
        }
        return RT_EOK;
    }
err:
    {
        LOG_E("--used Get_ADC_Voltage [0/1/2]");
        return RT_ERROR;
    }
}
MSH_CMD_EXPORT(TEST_Get_ADC_Voltage, 0:COIL 1:MQ-6 2:VIN);

