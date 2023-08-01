/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-07-25 17:08:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-07-27 12:46:40
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\inc\bsp_led.h
 * @Description  :
 *
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved.
 */
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "board.h"
#include "pin.h"
#include "drv_gpio.h"

#define LED_R_PIN               GET_PIN(A, 5)
#define LED_R_ON                rt_pin_write(LED_R_PIN, PIN_HIGH)
#define LED_R_OFF               rt_pin_write(LED_R_PIN, PIN_LOW)
#define READ_LED_R              rt_pin_read(LED_R_PIN)
#define LED_R_TOGGLE            rt_pin_write(LED_R_PIN, !rt_pin_read(LED_R_PIN))

#define LED_G_PIN               GET_PIN(B, 12)
#define LED_G_ON                rt_pin_write(LED_G_PIN, PIN_HIGH)
#define LED_G_OFF               rt_pin_write(LED_G_PIN, PIN_LOW)
#define READ_LED_G              rt_pin_read(LED_G_PIN)
#define LED_G_TOGGLE            rt_pin_write(LED_G_PIN, !rt_pin_read(LED_G_PIN))

#define LED_Y_PIN               GET_PIN(A, 4)
#define LED_Y_ON                rt_pin_write(LED_Y_PIN, PIN_HIGH)
#define LED_Y_OFF               rt_pin_write(LED_Y_PIN, PIN_LOW)
#define READ_LED_Y              rt_pin_read(LED_Y_PIN)
#define LED_Y_TOGGLE            rt_pin_write(LED_Y_PIN, !rt_pin_read(LED_Y_PIN))


#define NETLED_R_PIN               GET_PIN(B, 6)
#define NETLED_R_ON                rt_pin_write(NETLED_R_PIN, PIN_HIGH)
#define NETLED_R_OFF               rt_pin_write(NETLED_R_PIN, PIN_LOW)
#define READ_NETLED_R              rt_pin_read(NETLED_R_PIN)
#define NETLED_R_TOGGLE            rt_pin_write(NETLED_R_PIN, !rt_pin_read(NETLED_R_PIN))

#define NETLED_G_PIN               GET_PIN(A, 11)
#define NETLED_G_ON                rt_pin_write(NETLED_G_PIN, PIN_HIGH)
#define NETLED_G_OFF               rt_pin_write(NETLED_G_PIN, PIN_LOW)
#define READ_NETLED_G              rt_pin_read(NETLED_G_PIN)
#define NETLED_G_TOGGLE            rt_pin_write(NETLED_G_PIN, !rt_pin_read(NETLED_G_PIN))

#define NETLED_Y_PIN               GET_PIN(A, 12)
#define NETLED_Y_ON                rt_pin_write(NETLED_Y_PIN, PIN_HIGH)
#define NETLED_Y_OFF               rt_pin_write(NETLED_Y_PIN, PIN_LOW)
#define READ_NETLED_Y              rt_pin_read(NETLED_Y_PIN)
#define NETLED_Y_TOGGLE            rt_pin_write(NETLED_Y_PIN, !rt_pin_read(NETLED_Y_PIN))


int BSP_LED_Init(void);

#endif // !__BSP_LED_H__
