/*
 * @Author       : yzy
 * @Date         : 2023-01-30 15:40:07
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 11:50:38
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_mq.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_mq.h"
#include "bsp_adc.h"

#define LOG_TAG     "bsp_adc"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

TeMq Mq;

FlagStatus IS_MQ_EndOfLife(void)
{
    FlagStatus flag = RESET;

    if (RTC_GetCounter() >= Mq.expiration_seconds)
    {
        SysControl.Flag.mq_failure = 1;
        flag = SET;
    }

    return flag;
}

void BSP_MQ_Init(void)
{
    TuFlashProductTimeLimitFrame LimitTime;
    if (Flash_GetProductTimeLimit(&LimitTime, kExpirationTimeId) == READY)
    {
        Mq.expiration_seconds = DateTime2Seconds(LimitTime.Struct.year, \
            LimitTime.Struct.month, LimitTime.Struct.day, LimitTime.Struct.hour, \
            LimitTime.Struct.minute, LimitTime.Struct.second);
        logDebug("mq_expiration_time:%04d-%02d-%02d,%02d:%02d", \
                    LimitTime.Struct.year, LimitTime.Struct.month, LimitTime.Struct.day,
                    LimitTime.Struct.hour, LimitTime.Struct.minute, LimitTime.Struct.second);
    }
    // if (IS_MQ_EndOfLife() == SET)
    // {
    //     SysControl.Flag.mq_failure = SET;
    //     if (Flash_GetNum_Records(kRecordSensorFailure) == 0)
    //     {
    //         BSP_LED_Blink(kLedYellowId, 500);
    //         Flash_Write_Record(kRecordSensorFailure);
    //     }
    // }
    // if (Flash_GetMQ_AlarmValue() == 0)
    // {
    //     SysControl.Flag.mq_to_be_calib = 1;
    // }
    Mq.alarm_value = Flash_GetMQ_AlarmValue();
    if (Mq.alarm_value == 0)
    {
        SysControl.Flag.mq_to_be_calib = 1;
        Mq.alarm_value = MQ_VOLTAGE_ALARM_DEFAULT;
    }
    BSP_MQ_StartDetection();
    logDebug("Mq.alarm_value = %d", Mq.alarm_value);
}

uint16_t Get_MQ_VoltageInt1000x(void)
{
    uint16_t voltage = (Get_ADC_Average(kMqAdc) * 3.3 / 4096) * MQ_VOLTAGE_RATIO * 1000;

    // logDebug("Get_MQ_VoltageInt1000x = %04d", voltage);

    return voltage;
}
//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
//                Get_MQ_VoltageInt1000x, Get_MQ_VoltageInt1000x, "Get_MQ_VoltageInt1000x");

#if 0
TeMqStatus MQ_Detection(void)
{
    TeMqStatus mq_flag = kMqNormal;
    uint16_t mq_voltage = Get_MQ_VoltageInt1000x();

    // if (IS_MQ_EndOfLife() == SET)
    // {
    //     mq_flag = kMqEndOfLife;
    //     // logDebug("kMqEndOfLife");
    // }

    if (mq_voltage <= MQ_VOLTAGE_LOW_LIMIT)
    {
        mq_flag = kMqOpenCircuit;
        // logDebug("MQ_VOLTAGE_LOW_LIMIT");
    }
    else if (mq_voltage >= MQ_VOLTAGE_HIGH_LIMIT)
    {
        mq_flag = kMqShortCircuit;
        // logDebug("MQ_VOLTAGE_HIGH_LIMIT");
    }
    else if (mq_voltage >= Mq.alarm_value)
    {
        mq_flag = kMqAlarm;
        // logDebug("kMqAlarm");
    }

    if (mq_flag == kMqOpenCircuit || mq_flag == kMqShortCircuit)
    {
        mq_flag = kMqFault;
        // logDebug("kMqFault");
    }

    return mq_flag;
}
//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
                MQ_Detection, MQ_Detection, "mq_flag 0:kMqNormal");
#endif

TeMqStatus BSP_MQ_Detection(void)
{
    // uint8_t normal, alarm, fault;
    uint8_t alarm = 0, fault = 0;

    TeMqStatus flag = kMqNormal;

    for (uint8_t i = 0; i < MQ_SAMPLING_TIMS; i++)
    {
        // if (Mq.status_buffer[i] == kMqNormal)
        // {
        //     normal++;
        // }
        // else
        if (Mq.status_buffer[i] == kMqAlarm)
        {
            alarm++;
        }
        else if (Mq.status_buffer[i] == kMqFault)
        {
            fault++;
        }
    }

    // if (normal > MQ_THRESHOLD_NORMAL_TIMS)
    // {
    //     flag = kMqNormal;
    // }
    if (alarm > MQ_THRESHOLD_ALARM_TIMS)
    {
        flag = kMqAlarm;
    }
    else if (fault > MQ_THRESHOLD_FAULT_TIMS)
    {
        flag = kMqFault;
    }

    return flag;
}

void BSP_MQ_Loop(void)
{
    if (Mq.detection_flag == 1)
    {
        static uint8_t i;

        uint16_t mq_voltage = Get_MQ_VoltageInt1000x();
        logDebug("mq_voltage = %04d", mq_voltage);

        Mq.detection_flag = 0;

        if (mq_voltage > MQ_VOLTAGE_LOW_LIMIT
            && mq_voltage < Mq.alarm_value)
        {
            Mq.status_buffer[i] = kMqNormal;
        }
        else if (mq_voltage >= Mq.alarm_value)
        {
            // Mq.status_flag = kMqAlarm;
            Mq.status_buffer[i] = kMqAlarm;
            // logDebug("kMqAlarm");
        }
        else if (mq_voltage <= MQ_VOLTAGE_LOW_LIMIT)
        {
            Mq.status_buffer[i] = kMqFault;
            // Mq.status_flag = kMqOpenCircuit;
            // logDebug("MQ_VOLTAGE_LOW_LIMIT");
        }
        else if (mq_voltage >= MQ_VOLTAGE_HIGH_LIMIT)
        {
            Mq.status_buffer[i] = kMqFault;
            // Mq.status_flag = kMqShortCircuit;
            // logDebug("MQ_VOLTAGE_HIGH_LIMIT");
        }
        i++;
        if (i >= MQ_SAMPLING_TIMS)
        {
            i = 0;
        }
    }
}



