/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-07-25 17:08:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-03 17:16:52
 * @FilePath     : \JT-DT-YD1F01_RTT_Nano\bsp\inc\bsp_led.h
 * @Description  :
 *
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved.
 */
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "board.h"
#include "pin.h"
#include "drv_gpio.h"

#include "agile_led.h"

#define LED_THREAD_STACK_SIZE    (512)
#define LED_THREAD_PRIORITY      25
#define LED_THREAD_TIMESLICE     5

extern agile_led_t *led_r;
extern agile_led_t *led_g;
extern agile_led_t *led_y;

extern agile_led_t *netled_r;
extern agile_led_t *netled_g;
extern agile_led_t *netled_y;

#define LED_R_PIN               GET_PIN(A, 5)
// #define LED_R_ON                rt_pin_write(LED_R_PIN, PIN_HIGH)
// #define LED_R_OFF               rt_pin_write(LED_R_PIN, PIN_LOW)
// #define READ_LED_R              rt_pin_read(LED_R_PIN)
// #define LED_R_TOGGLE            rt_pin_write(LED_R_PIN, !rt_pin_read(LED_R_PIN))

#define LED_G_PIN               GET_PIN(B, 12)
// #define LED_G_ON                rt_pin_write(LED_G_PIN, PIN_HIGH)
// #define LED_G_OFF               rt_pin_write(LED_G_PIN, PIN_LOW)
// #define READ_LED_G              rt_pin_read(LED_G_PIN)
// #define LED_G_TOGGLE            rt_pin_write(LED_G_PIN, !rt_pin_read(LED_G_PIN))

#define LED_Y_PIN               GET_PIN(A, 4)
// #define LED_Y_ON                rt_pin_write(LED_Y_PIN, PIN_HIGH)
// #define LED_Y_OFF               rt_pin_write(LED_Y_PIN, PIN_LOW)
// #define READ_LED_Y              rt_pin_read(LED_Y_PIN)
// #define LED_Y_TOGGLE            rt_pin_write(LED_Y_PIN, !rt_pin_read(LED_Y_PIN))

#define NETLED_R_PIN               GET_PIN(B, 6)
// #define NETLED_R_ON                rt_pin_write(NETLED_R_PIN, PIN_HIGH)
// #define NETLED_R_OFF               rt_pin_write(NETLED_R_PIN, PIN_LOW)
// #define READ_NETLED_R              rt_pin_read(NETLED_R_PIN)
// #define NETLED_R_TOGGLE            rt_pin_write(NETLED_R_PIN, !rt_pin_read(NETLED_R_PIN))

#define NETLED_G_PIN               GET_PIN(A, 11)
// #define NETLED_G_ON                rt_pin_write(NETLED_G_PIN, PIN_HIGH)
// #define NETLED_G_OFF               rt_pin_write(NETLED_G_PIN, PIN_LOW)
// #define READ_NETLED_G              rt_pin_read(NETLED_G_PIN)
// #define NETLED_G_TOGGLE            rt_pin_write(NETLED_G_PIN, !rt_pin_read(NETLED_G_PIN))

#define NETLED_Y_PIN               GET_PIN(A, 12)
// #define NETLED_Y_ON                rt_pin_write(NETLED_Y_PIN, PIN_HIGH)
// #define NETLED_Y_OFF               rt_pin_write(NETLED_Y_PIN, PIN_LOW)
// #define READ_NETLED_Y              rt_pin_read(NETLED_Y_PIN)
// #define NETLED_Y_TOGGLE            rt_pin_write(NETLED_Y_PIN, !rt_pin_read(NETLED_Y_PIN))

typedef enum
{
    kLedRedId = 0u,
    kLedGreenId,
    kLedYellowId
}TeLedId;

int BSP_LED_Init(void);

#endif // !__BSP_LED_H__
