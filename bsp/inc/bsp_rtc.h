/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 14:25:51
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 14:25:51
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\inc\bsp_rtc.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

#include "board.h"

typedef struct __attribute__((packed))
{
    uint16_t year;     // 年份设置，取值范围为
    uint8_t month;    // 日期设置，取值范围为1~31
    uint8_t day;     // 月份设置，取值范围为1~12

    uint8_t hour;   /* 小时设置 */
    uint8_t minute; /* 分钟设置 */
    uint8_t second; /* 秒设置 */

    uint8_t week;  // 星期几设置，取值范围为1~7，对应星期一~星期日
} TsRtcDateTime;

extern TsRtcDateTime RtcDateTime;

uint8_t Is_LeapYear(uint16_t year);

uint32_t DateTime2Seconds(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);
void Seconds2DateTime(uint32_t seconds, TsRtcDateTime* pDateTime);

void RTC_Set_Time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);
void RTC_Set_Alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);

void RTC_Get_Time(void);
uint8_t RTC_Get_Week(uint16_t year, uint8_t month, uint8_t day);

uint8_t BSP_RTC_Init(void);
uint32_t RTC_Time2TimeStamp(void);

#endif // !__BSP_RTC_H__
