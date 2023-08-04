/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 11:46:56
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 11:46:56
 * @FilePath     : \JT-DT-YD1F01_RTT_Nano\bsp\inc\bsp_bkp.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_BKP_H__
#define __BSP_BKP_H__

#include "board.h"

//  注：后备数据寄存器(BKP DATARx) (x=11-42）适用于CH32F20x_D8、CH32F20x_D8C、CH32F20x_D8W、
// CH32V20x_D8、 CH32V20x_D8W、 CH32V30x_D8、 CH32V30x_D8C。
#define RTC_FIRST_INIT_VALUE        (0xA1A1)
#define BKP_DR_RTC_FIRST_INIT       BKP_DR1



#endif // !__BSP_BKP_H__
