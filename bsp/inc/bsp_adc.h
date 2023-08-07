/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 11:12:26
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-07 10:58:32
 * @FilePath     : \JT-DT-YD1F01_RTT_Nano\bsp\inc\bsp_adc.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "board.h"

// ADC要采集的通道个数
#define NUM_OF_CHANNEL              (3)
// ADC要采集的次数
#define ADC_TIMES                   (30)
#define ADC_BUFFER_SIZE             (NUM_OF_CHANNEL * ADC_TIMES)

// ADC GPIO 宏定义
#define USED_ADC_GPIO_PORT          GPIOA
#define USED_ADC_GPIO_CLK_ENABLE    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

// ADC 序号宏定义
#define USED_ADC                    ADC1
#define USED_ADC_CLK_ENABLE         RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

// ADC DMA 通道宏定义，这里我们使用DMA传输
#define USED_ADC_DR_ADDRESS         ((uint32_t)&USED_ADC->RDATAR)
#define USED_ADC_DMA_CLK_ENABLE     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#define USED_ADC_DMA_STREAM         DMA1_Channel1

/*****************机械手/电磁阀驱动电流的检测***********************/
// #define EMV_ADC_GPIO_PORT              GPIOA
#define EMV_ADC_GPIO_PIN               GPIO_Pin_0
// #define EMV_ADC_GPIO_CLK_ENABLE         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

// #define EMV_ADC                        ADC1
// #define EMV_ADC_CLK_ENABLE              RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
#define EMV_ADC_CHANNEL                ADC_Channel_0

/****************************MQ-6的检测****************************/
// #define MQ_ADC_GPIO_PORT              GPIOA
#define MQ_ADC_GPIO_PIN               GPIO_Pin_1
// #define MQ_ADC_GPIO_CLK_ENABLE         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

// #define MQ_ADC                        ADC1
// #define MQ_ADC_CLK_ENABLE              RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
#define MQ_ADC_CHANNEL                ADC_Channel_1

/***************************12V输入的检测，用来实现掉电检测****************************/
// #define VIN_ADC_GPIO_PORT              GPIOA
#define VIN_ADC_GPIO_PIN               GPIO_Pin_7
// #define VIN_ADC_GPIO_CLK_ENABLE         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

// #define VIN_ADC                        ADC1
// #define VIN_ADC_CLK_ENABLE              RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
#define VIN_ADC_CHANNEL                ADC_Channel_7

typedef enum
{
    kCoilAdc = 0U,
    kMqAdc,
    kVinAdc
} TeAdcIndex;

int BSP_ADC_Init(void);

float Get_ADC_Average(TeAdcIndex index);
float Get_ADC_Voltage(TeAdcIndex index);
uint16_t Get_ADC_VoltageInt1000x(TeAdcIndex index);

#endif // !__BSP_ADC_H__
