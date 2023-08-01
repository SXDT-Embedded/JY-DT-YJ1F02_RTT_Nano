/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-07-26 08:38:33
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-07-27 13:00:22
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\inc\bsp_buzzer.h
 * @Description  :
 *
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved.
 */
#ifndef __BSP_BUZZER_H__
#define __BSP_BUZZER_H__

#include "board.h"
#include "pin.h"
#include "drv_gpio.h"

#define BUZZER_PIN               GET_PIN(B, 15)
#define BUZZER_ON                rt_pin_write(BUZZER_PIN, PIN_HIGH)
#define BUZZER_OFF               rt_pin_write(BUZZER_PIN, PIN_LOW)
#define READ_BUZZER              rt_pin_read(BUZZER_PIN)
#define BUZZER_TOGGLE            rt_pin_write(BUZZER_PIN, !rt_pin_read(BUZZER_PIN))


int BSP_BUZZER_Init(void);

#endif // !__BSP_BUZZER_H__
