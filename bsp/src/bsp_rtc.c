/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 14:25:55
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 14:27:12
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_rtc.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_rtc.h"
#include "bsp_wdg.h"
#include "bsp_bkp.h"
#include "bsp_flash.h"

#define LOG_TAG     "bsp_rtc"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

void RTC_IRQHandler(void) __attribute__((interrupt("")));

const uint8_t month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

TsRtcDateTime RtcDateTime;

/**
 * @description: Judging whether it is a leap year.
 * @param {uint16_t} year
 * @return {*}  1 - Yes
 *              0 - No
 */
//  月份    1  2  3  4  5  6  7  8  9  10 11 12
//  闰年    31 29 31 30 31 30 31 31 30 31 30 31
//  非闰年  31 28 31 30 31 30 31 31 30 31 30 31
uint8_t Is_LeapYear(uint16_t year)
{
    if(year % 4 == 0)
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)
                return 1;
            else
                return 0;
        }
        else
            return 1;
    }
    else
        return 0;
}

/**
 * @description: 将年月日时分秒转换成秒钟数
 *   @note      以1970年1月1日为基准, 1970年1月1日, 0时0分0秒, 表示第0秒钟
 *              最大表示到2105年, 因为uint32_t最大表示136年的秒钟数(不包括闰年)!
 *              本代码参考只linux mktime函数, 原理说明见此贴:
 *              http://www.openedv.com/thread-63389-1-1.html
 * @param {uint16_t} syear
 * @param {uint8_t} smon
 * @param {uint8_t} sday
 * @param {uint8_t} hour
 * @param {uint8_t} min
 * @param {uint8_t} sec
 * @return {*} 转换后的秒钟数
 */
uint32_t DateTime2Seconds(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    logAssert((syear > 1970 || syear < 2099), printf("DateTime2Seconds(): [syear]Error!"));

    uint32_t y, m, d, x, t;
    signed char monx = smon;    /* 将月份转换成带符号的值, 方便后面运算 */

    if (0 >= (monx -= 2))       /* 1..12 -> 11,12,1..10 */
    {
        monx += 12;             /* Puts Feb last since it has leap day */
        syear -= 1;
    }

    y = (syear - 1) * 365 + syear / 4 - syear / 100 + syear / 400; /* 公元元年1到现在的闰年数 */
    m = 367 * monx / 12 - 30 + 59;
    d = sday - 1;
    x = y + m + d - 719162;                      /* 减去公元元年到1970年的天数 */
    t = ((x * 24 + hour) * 60 + min) * 60 + sec; /* 总秒钟数 */
    return t;
}

uint32_t RTC_Time2TimeStamp(void)
{
    RTC_Get_Time();
    uint32_t time_stamp = DateTime2Seconds(RtcDateTime.year, RtcDateTime.month, RtcDateTime.day
              , RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second) - (8 * 3600);

    return time_stamp;
}

void Seconds2DateTime(uint32_t seconds, TsRtcDateTime* pDateTime)
{
    uint16_t daycnt = 0;
    uint32_t        timecount = 0;
    uint32_t        temp = 0;
    uint16_t        temp1 = 0;

    timecount = seconds;
    temp = timecount / 86400;
    if(daycnt != temp)
    {
        daycnt = temp;
        temp1 = 1970;
        while(temp >= 365)
        {
            if(Is_LeapYear(temp1))
            {
                if(temp >= 366)
                    temp -= 366;
                else
                {
                    temp1++;
                    break;
                }
            }
            else
                temp -= 365;
            temp1++;
        }
        pDateTime->year = temp1;
        temp1 = 0;
        while(temp >= 28)
        {
            if(Is_LeapYear(pDateTime->year) && temp1 == 1)
            {
                if(temp >= 29)
                    temp -= 29;
                else
                    break;
            }
            else
            {
                if(temp >= month_table[temp1])
                    temp -= month_table[temp1];
                else
                    break;
            }
            temp1++;
        }
        pDateTime->month = temp1 + 1;
        pDateTime->day = temp + 1;
    }
    temp = timecount % 86400;
    pDateTime->hour = temp / 3600;
    pDateTime->minute = (temp % 3600) / 60;
    pDateTime->second = (temp % 3600) % 60;
    pDateTime->week = RTC_Get_Week(pDateTime->year, pDateTime->month, pDateTime->day);
}

/**
 * @description: 设置时间, 包括年月日时分秒
 *   @note      以1970年1月1日为基准, 往后累加时间
 *              合法年份范围为: 1970 ~ 2105年
                HAL默认为年份起点为2000年
 * @param {uint16_t} syear
 * @param {uint8_t} smon
 * @param {uint8_t} sday
 * @param {uint8_t} hour
 * @param {uint8_t} min
 * @param {uint8_t} sec
 * @return {*}
 */
void RTC_Set_Time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    RTC_SetCounter(DateTime2Seconds(syear, smon, sday, hour, min, sec));
    RTC_WaitForLastTask();

    RTC_Get_Time();
    logDebug("%4d-%02d-%02d, %02d:%02d:%02d", RtcDateTime.year, RtcDateTime.month, RtcDateTime.day, \
            RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second);
}
//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), \
        RTC_Set_Time, RTC_Set_Time, "y m d h m s");

/**
 * @description: Set Alarm Time.
 * @param {uint16_t} syear
 * @param {uint8_t} smon
 * @param {uint8_t} sday
 * @param {uint8_t} hour
 * @param {uint8_t} min
 * @param {uint8_t} sec
 * @return {*}
 */
void RTC_Set_Alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    // 使能电源时钟、使能备份域时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE); // 取消备份域写保护

    RTC_SetAlarm(DateTime2Seconds(syear, smon, sday, hour, min, sec));
    RTC_WaitForLastTask();
}

/**
 * @description: 该函数不直接返回时间, 时间数据保存在RtcDateTime结构体里面
 * @return {*}
 */
void RTC_Get_Time(void)
{
    uint16_t daycnt = 0;
    uint32_t        timecount = 0;
    uint32_t        temp = 0;
    uint16_t        temp1 = 0;

    timecount = RTC_GetCounter();
    temp = timecount / 86400;
    if(daycnt != temp)
    {
        daycnt = temp;
        temp1 = 1970;
        while(temp >= 365)
        {
            if(Is_LeapYear(temp1))
            {
                if(temp >= 366)
                    temp -= 366;
                else
                {
                    temp1++;
                    break;
                }
            }
            else
                temp -= 365;
            temp1++;
        }
        RtcDateTime.year = temp1;
        temp1 = 0;
        while(temp >= 28)
        {
            if(Is_LeapYear(RtcDateTime.year) && temp1 == 1)
            {
                if(temp >= 29)
                    temp -= 29;
                else
                    break;
            }
            else
            {
                if(temp >= month_table[temp1])
                    temp -= month_table[temp1];
                else
                    break;
            }
            temp1++;
        }
        RtcDateTime.month = temp1 + 1;
        RtcDateTime.day = temp + 1;
    }
    temp = timecount % 86400;
    RtcDateTime.hour = temp / 3600;
    RtcDateTime.minute = (temp % 3600) / 60;
    RtcDateTime.second = (temp % 3600) % 60;
    RtcDateTime.week = RTC_Get_Week(RtcDateTime.year, RtcDateTime.month, RtcDateTime.day);
}

/**
 * @description: Get the current day of the week.
 *              输入公历日期得到星期(起始时间为: 公元0年3月1日开始, 输入往后的任何日期, 都可以获取正确的星期)
 *              使用 基姆拉尔森计算公式 计算, 原理说明见此贴:
 *              https://www.cnblogs.com/fengbohello/p/3264300.html
 * @param {uint16_t} year
 * @param {uint8_t} month
 * @param {uint8_t} day
 * @return {*}
 */
uint8_t RTC_Get_Week(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t week = 0;

    if (month < 3)
    {
        month += 12;
        --year;
    }

    week = (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + (year >> 2) - year / 100 + year / 400) % 7;
    return week;
}

/**
 * @description: Initializes RTC Int.
 * @return {*}
 */
static void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static uint8_t RTC_First_Init(void)
{
    uint8_t temp = 0;

    RCC_LSEConfig(RCC_LSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250)
    {
        temp++;
//        logDebug("temp: %d", temp);
        Delay_Ms(5);
    }
    if(temp >= 250)
        return 1;

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForLastTask();
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    RTC_SetPrescaler(32767);
    RTC_WaitForLastTask();

    // RTC_ITConfig(RTC_IT_ALR, ENABLE);
    // RTC_ITConfig(RTC_IT_SEC, ENABLE);
    // RTC_WaitForLastTask();

    /* Is it the first configuration */
    BKP_WriteBackupRegister(BKP_DR_RTC_FIRST_INIT, RTC_FIRST_INIT_VALUE);

    TuFlashProductTimeLimitFrame LimitTime;
    if (Flash_GetProductTimeLimit(&LimitTime, kFactoryTimeId) == READY)
    {
        RTC_Set_Time(LimitTime.Struct.year, LimitTime.Struct.month, LimitTime.Struct.day, \
                     LimitTime.Struct.hour, LimitTime.Struct.minute, 0); /* Setup Time */
        logDebug("RTC_Set_Time:%04d-%02d-%02d,%02d:%02d", \
                    LimitTime.Struct.year, LimitTime.Struct.month, LimitTime.Struct.day,
                    LimitTime.Struct.hour, LimitTime.Struct.minute);
    }

    return 0;
}
////SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), \
//        RTC_First_Init, RTC_First_Init, "RTC_First_Init");

/**
 * @description: Initializes RTC collection.
 * @return {*}  1 - Init Fail  0 - Init Success
 */
// https://blog.csdn.net/Dreamer_HHH/article/details/95636976
uint8_t BSP_RTC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    logDebug("RCC_GetFlagStatus(RCC_FLAG_LSERDY): %d", RCC_GetFlagStatus(RCC_FLAG_LSERDY));
    if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET || BKP_ReadBackupRegister(BKP_DR_RTC_FIRST_INIT) != RTC_FIRST_INIT_VALUE)
    {
        RTC_First_Init();
    }
    else
    {
        // 外部手动复位
        // if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        // {
        //     logDebug("外部手动复位");
        // }
        // // 上电/掉电复位标志
        // if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        // {
        //     logDebug("上电/掉电复位");
        //     Flash_Write_Record(kRecordPowerOn);
        //     RCC_ClearFlag();
        // }
        // 软件复位
        if(RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
        {
            logDebug("软件复位");
        }
        // 独立看门狗复位
        if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
        {
            logDebug("独立看门狗复位");
        }
        // 窗口看门狗复位
        if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
        {
            logDebug("窗口看门狗复位");
        }
        // 低功耗复位
        if(RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
        {
            logDebug("低功耗复位");
        }
        // RCC_ClearFlag();

        // 虽然RTC模块不需要重新配置，且掉电后依靠后备电池依然运行
        // 但是每次上电后，还是要使能RTCCLK
        RCC_RTCCLKCmd(ENABLE);  // 使能RTCCLK
        RTC_WaitForSynchro();   // 等待RTC时钟与APB1时钟同步
        // RTC_ITConfig(RTC_IT_ALR, ENABLE);
        // RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }

    RTC_NVIC_Config();
    RTC_Get_Time();

    logDebug("%4d-%02d-%02d, %02d:%02d:%02d", RtcDateTime.year, RtcDateTime.month, RtcDateTime.day, \
            RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second);
    logDebug("BSP_RTC_Init!");

    return 0;
}

void RTC_Control(uint8_t flag)
{
    if(flag == 0)
    {
        RTC_ITConfig(RTC_IT_SEC, DISABLE);
        RTC_WaitForLastTask();
    }
    else if(flag == 1)
    {
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
    else if(flag == 2)
    {
        RTC_Get_Time();
        logDebug("%4d-%02d-%02d, week:%d, %02d:%02d:%02d", RtcDateTime.year, RtcDateTime.month, RtcDateTime.day, \
                   RtcDateTime.week ,RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second);
    }
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), \
        RTC_Control, RTC_Control, "0, 1(1s)show time: 2-show time once");

/**
 * @brief       RTC时钟中断
 *   @note      秒钟中断服务函数,顺带处理闹钟标志
 *              根据RTC_CRL寄存器的 SECF 和 ALRF 位区分是哪个中断
 * @param       无
 * @retval      无
 */
void RTC_IRQHandler(void)
{
    // 秒中断
    if(RTC_GetITStatus(RTC_FLAG_SEC) != RESET)
    {
        RTC_ClearITPendingBit(RTC_FLAG_SEC);
        RTC_Get_Time();
        logDebug("%4d-%02d-%02d, %02d:%02d:%02d", RtcDateTime.year, RtcDateTime.month, RtcDateTime.day, \
            RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second);
        // if (IS_MQ_EndOfLife() == SET)
        // {
        //     SysControl.Flag.mq_failure = SET;
        // }
    }

    // 闹钟中断
    // if(RTC_GetITStatus(RTC_FLAG_ALR) != RESET)
    // {
    //     RTC_Get_Time();
    //     RTC_ClearITPendingBit(RTC_FLAG_ALR);
    //     logDebug("%4d-%2d-%2d,%2d:%2d:%2d", RtcDateTime.year, RtcDateTime.month, RtcDateTime.date, \
    //         RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second);
    // }

    // if(RTC_GetITStatus(RTC_FLAG_OW) != RESET)
    // {
    //     // 清除溢出中断标志
    //     RTC_ClearITPendingBit(RTC_FLAG_OW);
    //     RTC_WaitForLastTask();
    // }
}
