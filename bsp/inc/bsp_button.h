/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 17:15:14
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-04 16:47:04
 * @FilePath     : \JY-DT-YJ1F02_RTT_Nano\bsp\inc\bsp_button.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_BUTTON_H__
#define __BSP_BUTTON_H__

#include "board.h"
#include "pin.h"
#include "drv_gpio.h"

#define BUTTON_MUTE_CHECK_PIN                 GET_PIN(A, 6)
#define READ_BUTTON_MUTE_CHECK                rt_pin_read(BUTTON_MUTE_CHECK_PIN)

// 不然就是用 timer 实现按键扫描处理
// #define BUTTON_USE_THREAD

#ifdef BUTTON_USE_THREAD
#define BUTTON_THREAD_STACK_SIZE    (768)
#define BUTTON_THREAD_PRIORITY      (15)
#define BUTTON_THREAD_TIMESLICE     (5)
#endif // !BUTTON_USE_THREAD


typedef enum
{
  kButtonMuteCheckId = 0U,
}TeButtonId;

int BSP_BUTTON_Init(void);

#endif // !__BSP_BUTTON_H__
