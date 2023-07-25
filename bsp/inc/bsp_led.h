/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-07-25 17:08:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-07-25 17:15:00
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\inc\bsp_led.h
 * @Description  :
 *
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved.
 */
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "board.h"

#define LED_R_PIN                   GPIO_Pin_5
#define LED_R_GPIO_PORT             GPIOA
#define LED_R_GPIO_CLK_ENABLE       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

#define LED_G_PIN                   GPIO_Pin_12
#define LED_G_GPIO_PORT             GPIOB
#define LED_G_GPIO_CLK_ENABLE       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)

#define LED_Y_PIN                   GPIO_Pin_4
#define LED_Y_GPIO_PORT             GPIOA
#define LED_Y_GPIO_CLK_ENABLE       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

#define LED_R_ON            GPIO_WriteBit(LED_R_GPIO_PORT, LED_R_PIN, Bit_SET)
#define LED_R_OFF           GPIO_WriteBit(LED_R_GPIO_PORT, LED_R_PIN, Bit_RESET)
#define READ_LED_R          GPIO_ReadOutputDataBit(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_TOGGLE        GPIO_WriteBit(LED_R_GPIO_PORT, LED_R_PIN, !READ_LED_R)

#define LED_G_ON            GPIO_WriteBit(LED_G_GPIO_PORT, LED_G_PIN, Bit_SET)
#define LED_G_OFF           GPIO_WriteBit(LED_G_GPIO_PORT, LED_G_PIN, Bit_RESET)
#define READ_LED_G          GPIO_ReadOutputDataBit(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_TOGGLE        GPIO_WriteBit(LED_G_GPIO_PORT, LED_G_PIN, !READ_LED_G)

#define LED_Y_ON            GPIO_WriteBit(LED_Y_GPIO_PORT, LED_Y_PIN, Bit_SET)
#define LED_Y_OFF           GPIO_WriteBit(LED_Y_GPIO_PORT, LED_Y_PIN, Bit_RESET)
#define READ_LED_Y          GPIO_ReadOutputDataBit(LED_Y_GPIO_PORT, LED_Y_PIN)
#define LED_Y_TOGGLE        GPIO_WriteBit(LED_Y_GPIO_PORT, LED_Y_PIN, !READ_LED_Y)


int BSP_LED_Init(void);

#endif // !__BSP_LED_H__
