/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 17:51:11
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 17:51:12
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\inc\bsp_vin_detection.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_VIN_DETECTION_H__
#define __BSP_VIN_DETECTION_H__

#include "board.h"

#define VIN_VOLTAGE_RATIO        (4.66666666F)

// 不含输入二极管的压降
// (30/140) * 14V = 3V, 3/3.3*4096 = 3723
#define VIN_ADC_HIGH_THRESHOLD      (uint16_t)3723  // 14V
// (30/140) * 10V = 2.1429V, 2.1429/3.3*4096 = 2659
#define VIN_ADC_LOW_THRESHOLD       (uint16_t)2127  // 8V


float Get_VIN_Voltage(void);
uint16_t Get_VIN_VoltageInt1000x(void);

int BSP_VIN_Detection_Init(void);

#endif // !__BSP_VIN_DETECTION_H__
