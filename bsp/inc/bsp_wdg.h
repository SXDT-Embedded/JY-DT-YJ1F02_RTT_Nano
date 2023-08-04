/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 14:27:22
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-03 13:00:17
 * @FilePath     : \JT-DT-YD1F01_RTT_Nano\bsp\inc\bsp_wdg.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_WDG_H__
#define __BSP_WDG_H__

#include "board.h"

#define WDG_THREAD_STACK_SIZE    (320)
#define WDG_THREAD_PRIORITY      30
#define WDG_THREAD_TIMESLICE     5

// 暂时只写了IWDG的
#define USED_IWDG   1
#define USED_WWDG   0

#if USED_IWDG
#define IWDG_FEED_INTERVAL_MS    (500U)

void IWDG_Init(uint16_t prescaler, uint16_t reload);
void IWDG_Feed(void);

#elif USED_WWDG
void WWDG_Init(uint32_t prv, uint8_t wr, uint8_t tr);
void WWDG_Feed(void);
#endif // !USED_IWDG

int BSP_WDG_Init(void);

#endif // !__BSP_WDG_H__
