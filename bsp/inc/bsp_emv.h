/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 15:44:20
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 16:38:02
 * @FilePath     : \JT-DT-YD1F01_RTT_Nano\bsp\inc\bsp_emv.h
 * @Description  : 电磁阀 EMV
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_EMV_H__
#define __BSP_EMV_H__

#include "board.h"
#include "pin.h"
#include "drv_gpio.h"

#define EMV_THREAD_PRIORITY    2
#define EMV_THREAD_TIMESLICE   5

#define IS_USED_EMV    1

// 电流采样电阻
#define EMV_CSR     (float)(0.2)

// 电流采样间隔 ms
// #define EMV_CSI_MS         (100U)

// 电磁阀动作时间
#define EMV_ACTION_MS       (uint8_t)(10U)

#define EMV_A_PIN              GET_PIN(B, 13)
#define EMV_A_LOW              rt_pin_write(EMV_A_PIN, PIN_LOW)
#define EMV_A_HIGH             rt_pin_write(EMV_A_PIN, PIN_HIGH)

#define EMV_B_PIN              GET_PIN(B, 14)
#define EMV_B_LOW              rt_pin_write(EMV_B_PIN, PIN_LOW)
#define EMV_B_HIGH             rt_pin_write(EMV_B_PIN, PIN_HIGH)

typedef enum
{
    kCoast = 0,
    kForward,
    kBackward,
    kBrake
} TeCoilDirection;

typedef enum
{
    kClose = 0,
    kOpen
} TeCoilControl;

void Coil_Run(TeCoilDirection direction);
float Get_Coil_Current(void);
int Coil_Close(void);

int BSP_Coil_Init(void);












#endif // !__BSP_EMV_H__
