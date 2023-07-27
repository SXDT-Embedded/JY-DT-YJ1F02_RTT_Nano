/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-07-26 08:38:33
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-07-26 08:38:33
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\inc\bsp_buzzer.h
 * @Description  :
 *
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved.
 */
#ifndef __BSP_BUZZER_H__
#define __BSP_BUZZER_H__

#include "board.h"

#define BUZZER_GPIO_PIN           GPIO_Pin_15
#define BUZZER_GPIO_PORT          GPIOB
#define BUZZER_GPIO_CLK_ENABLE    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)

#define BUZZER_ON           GPIO_WriteBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, Bit_SET)
#define BUZZER_OFF          GPIO_WriteBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, Bit_RESET)
#define BUZZER_TOGGLE       GPIO_WriteBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, !GPIO_ReadOutputDataBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN))


#endif // !__BSP_BUZZER_H__
