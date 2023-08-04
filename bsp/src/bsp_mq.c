/*
 * @Author       : yzy
 * @Date         : 2023-01-30 15:40:07
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-04 08:52:10
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_mq.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_mq.h"
#include "bsp_adc.h"

#define LOG_TAG     "bsp_mq"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

/* mq事件控制块 */
struct rt_event mq_event;

ALIGN(RT_ALIGN_SIZE)
static char mq_detection_thread_stack[MQ_THREAD_STACK_SIZE];
static struct rt_thread mq_detection_thread;

TeMq Mq;

// FlagStatus IS_MQ_EndOfLife(void)
// {
//     FlagStatus flag = RESET;

//     if (RTC_GetCounter() >= Mq.expiration_seconds)
//     {
//         SysControl.Flag.mq_failure = 1;
//         flag = SET;
//     }

//     return flag;
// }

uint16_t Get_MQ_VoltageInt1000x(void)
{
    uint16_t voltage = (Get_ADC_Average(kMqAdc) * 3.3 / 4096) * MQ_VOLTAGE_RATIO * 1000;
    // LOG_D("Get_MQ_VoltageInt1000x = %04d", voltage);

    return voltage;
}

static void TEST_Get_MQ_VoltageInt1000x(void)
{
    uint16_t voltage = (Get_ADC_Average(kMqAdc) * 3.3 / 4096) * MQ_VOLTAGE_RATIO * 1000;
    LOG_D("Get_MQ_VoltageInt1000x = %04d", voltage);
}
MSH_CMD_EXPORT(TEST_Get_MQ_VoltageInt1000x, TEST_Get_MQ_VoltageInt1000x);

// MQ检测线程函数
static void mq_detection_thread_entry(void *param)
{
    uint8_t i = 0;
    uint8_t alarm = 0, short_circuit = 0, open_circuit = 0;

    Mq.alarm_value = MQ_VOLTAGE_ALARM_DEFAULT;

    while (1)
    {
        uint16_t mq_voltage = Get_MQ_VoltageInt1000x();
        // LOG_D("mq_voltage = %04d", mq_voltage);
        // LOG_D("i = %d", i);

        if (mq_voltage >= Mq.alarm_value)
        {
            alarm++;
        }
        else if (mq_voltage <= MQ_VOLTAGE_LOW_LIMIT)
        {
            short_circuit++;
        }
        else if (mq_voltage >= MQ_VOLTAGE_HIGH_LIMIT)
        {
            open_circuit++;
        }

        if (alarm > MQ_THRESHOLD_ALARM_TIMS)
        {
            rt_event_send(&mq_event, MQ_EVENT_ALARM_FLAG);
            // LOG_D("send MQ_EVENT_ALARM_FLAG");
        }
        else if (short_circuit > MQ_THRESHOLD_FAULT_TIMS)
        {
            rt_event_send(&mq_event, MQ_EVENT_SHORT_CIRCUIT_FLAG);
            LOG_D("send MQ_EVENT_SHORT_CIRCUIT_FLAG");
        }
        else if (open_circuit > MQ_THRESHOLD_FAULT_TIMS)
        {
            rt_event_send(&mq_event, MQ_EVENT_OPEN_CIRCUIT_FLAG);
            LOG_D("send MQ_EVENT_OPEN_CIRCUIT_FLAG");
        }

        i++;
        if (i >= MQ_SAMPLING_TIMS)
        {
            i = 0;
            alarm = 0;
            short_circuit = 0;
            open_circuit = 0;
        }

        rt_thread_mdelay(MQ_SAMPLING_MS);
    }
}

int BSP_MQ_Init(void)
{
    // TuFlashProductTimeLimitFrame LimitTime;
    // if (Flash_GetProductTimeLimit(&LimitTime, kExpirationTimeId) == READY)
    // {
    //     Mq.expiration_seconds = DateTime2Seconds(LimitTime.Struct.year, \
    //         LimitTime.Struct.month, LimitTime.Struct.day, LimitTime.Struct.hour, \
    //         LimitTime.Struct.minute, LimitTime.Struct.second);
    //     logDebug("mq_expiration_time:%04d-%02d-%02d,%02d:%02d", \
    //                 LimitTime.Struct.year, LimitTime.Struct.month, LimitTime.Struct.day,
    //                 LimitTime.Struct.hour, LimitTime.Struct.minute, LimitTime.Struct.second);
    // }
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
    // Mq.alarm_value = Flash_GetMQ_AlarmValue();
    // if (Mq.alarm_value == 0)
    // {
    //     SysControl.Flag.mq_to_be_calib = 1;
    //     Mq.alarm_value = MQ_VOLTAGE_ALARM_DEFAULT;
    // }

    rt_err_t result;

    /* 初始化MQ事件对象 */
    result = rt_event_init(&mq_event, "mq_event", RT_IPC_FLAG_PRIO);
    if (result != RT_EOK)
    {
        LOG_D("init mq_event failed");
        return -1;
    }

    rt_thread_init(&mq_detection_thread,
                   "mq_detection_thread",
                   mq_detection_thread_entry,
                   RT_NULL,
                   &mq_detection_thread_stack[0],
                   sizeof(mq_detection_thread_stack),
                   MQ_THREAD_PRIORITY, MQ_THREAD_TIMESLICE);
    rt_thread_startup(&mq_detection_thread);

    LOG_I("BSP_MQ_Init");

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_DEVICE_EXPORT(BSP_MQ_Init);
#endif

static void TEST_MQ_Send()
{
    rt_event_send(&mq_event, MQ_EVENT_ALARM_FLAG);
    rt_thread_mdelay(10);
    rt_event_send(&mq_event, MQ_EVENT_END_OF_LIFE_FLAG);
    rt_thread_mdelay(10);
    rt_event_send(&mq_event, MQ_EVENT_OPEN_CIRCUIT_FLAG);
    rt_thread_mdelay(10);
    rt_event_send(&mq_event, MQ_EVENT_SHORT_CIRCUIT_FLAG);
    rt_thread_mdelay(10);
}
MSH_CMD_EXPORT(TEST_MQ_Send, TEST_MQ_Send);

