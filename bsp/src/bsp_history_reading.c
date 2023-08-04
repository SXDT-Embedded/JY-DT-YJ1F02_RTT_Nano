/*
 * @Author       : yzy
 * @Date         : 2023-02-01 11:59:45
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-04 09:26:40
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_history_reading.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_history_reading.h"
#include "bsp_flash.h"
#include "bsp_rtc.h"

#include "drv_uart.h"
// #include "bsp_air780.h"

#define LOG_TAG     "bsp_history_reading"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

ALIGN(RT_ALIGN_SIZE)
static char hr_thread_stack[HR_THREAD_STACK_SIZE];
static struct rt_thread hr_thread;

/**
 * @description: 从帧起始符开始到校验码之前所有字节的和的模256，即各字节不计超过255的溢出值的二进制算术和。
 * @param {uint8_t} *data
 * @param {uint16_t} len
 * @return {*}
 */
static uint8_t HR_CheckSum(const uint8_t *data, size_t len)
{
    uint8_t sum = 0;

    while (len--)
    {
        // LOG_D("*data = %d", *data);
        sum += *(data++);
    }
    // sum = sum & (0xFF);

    // LOG_D("sum = %02x", sum);

    // return (uint8_t)(sum & 0xFF);
    return sum;
}

/**
 * @description: 从接收缓冲区中获取一帧有效数据
 * @param {TsFrameData} *pFrameData 主机帧
 * @param {uint8_t} *p_src      接收缓冲区
 * @param {uint16_t} src_len    接收缓冲区长度
 * @return {*}
 */
TsFrameData* HR_GetFrameData(const uint8_t *p_src, const uint8_t src_len)
{
    uint8_t data_field_len = 0;
    uint8_t check_sum = 0;

    TsFrameData *get_buffer = RT_NULL;

    for(size_t i = 0; i < src_len; i++)
    {
        if(p_src[i] == FRAME_HEADER)
        {
            data_field_len = p_src[i + 3];

            if(p_src[i + 3 + data_field_len + 2] == FRAME_TAIL)
            {
                check_sum = HR_CheckSum(&p_src[i] , (4 + data_field_len));

                if(p_src[i + 3 + data_field_len + 1] == check_sum)
                {
                    get_buffer = (TsFrameData *)rt_malloc(sizeof(TsFrameData) + sizeof(uint8_t) * data_field_len);

                    get_buffer->c1 = p_src[i + 1];
                    get_buffer->c2 = p_src[i + 2];
                    get_buffer->len = data_field_len;

                    if(data_field_len > 0)
                    {
                        // p_data = &p_src[i + 4];
                        rt_strncpy(get_buffer->data, &p_src[i + 4], data_field_len);
                    }
                    LOG_D("HR_GetDataFrame Success!");

                    return get_buffer;
                }
            }
        }
    }
    LOG_D("HR_GetDataFrame Fail!");

    return get_buffer;
}


uint8_t HR_GenerateRawFrame(TsRawFrameData* pRawData
    , uint8_t c1, TeFrameC2 c2, const uint8_t* p_src, uint8_t src_len)
{
    uint8_t flag = 0;

    pRawData->len = src_len + 6;

    rt_memset(pRawData->buf, 0 , sizeof(pRawData->buf));

    pRawData->buf[0] = FRAME_HEADER;
    pRawData->buf[1] = c1;
    pRawData->buf[2] = c2;
    pRawData->buf[3] = src_len;

    rt_memmove(&pRawData->buf[4], p_src, src_len);

    // 从帧起始符开始到校验码之前所有字节的和的模256
    // ，即各字节不计超过255的溢出值的二进制算术和。
    pRawData->buf[pRawData->len - 2] = HR_CheckSum(&pRawData->buf[0], pRawData->len - 2);
    pRawData->buf[pRawData->len - 1] = FRAME_TAIL;

    LOG_HEX("HrRawData", 16, &pRawData->buf[0], pRawData->len);

    return flag;
}

uint8_t HR_ProcessData(const TsFrameData *pHostFrameData, TeDataSources from)
{
    TuFlashProductTimeLimitFrame ReadLimitTime;

    uint8_t flag = 0;
    uint8_t c1, c2, data_len;
    uint8_t data_field[256] = {0};

    c1 = pHostFrameData->c1;
    c2 = pHostFrameData->c2;

    switch (c2)
    {
    // 查询各类记录的总数
    case kNumOfRecords:
        LOG_I("kNumOfRecords");
        c1 = 0;
        data_len = sizeof(TsTotalRecords);
        if(Flash_GetTotalRecord((TsTotalRecords *)data_field) != RESET)
        {
            flag = 1;
        }
        break;
    // 查询第n条探测器报警记录
    case kAlarmRecord:
        LOG_I("kAlarmRecord(%d)", c1);
        data_len = sizeof(TsRecordsTime);
        if(Flash_GetRecord(kAlarmRecord, c1, (TsRecordsTime *)data_field) != RESET)
        {
            flag = 1;
        }
        break;
    // 查询第n条探测器报警恢复记录
    case kAlarmRcyRecord:
        LOG_I("kAlarmRcyRecord(%d)", c1);
        data_len = sizeof(TsRecordsTime);
        if(Flash_GetRecord(kAlarmRcyRecord, c1, (TsRecordsTime *)data_field) != RESET)
        {
            flag = 1;
        }
        break;
    // 查询第n条探测器故障记录
    case kFaultRecord:
        LOG_I("kFaultRecord(%d)", c1);
        data_len = sizeof(TsRecordsTime);
        if(Flash_GetRecord(kFaultRecord, c1, (TsRecordsTime *)&data_field) != RESET)
        {
            flag = 1;
        }
        break;
    // 查询第n条探测器故障恢复记录
    case kFaultRcyRecord:
        LOG_I("kFaultRcyRecord(%d)", c1);
        data_len = sizeof(TsRecordsTime);
        if(Flash_GetRecord(kFaultRcyRecord, c1, (TsRecordsTime *)data_field) != RESET)
        {
            flag = 1;
        }
        break;
    // 查询第n条探测器掉电记录
    case kPowerFailureRecord:
        LOG_I("kPowerFailureRecord(%d)", c1);
        data_len = sizeof(TsRecordsTime);
        if(Flash_GetRecord(kPowerFailureRecord, c1, (TsRecordsTime *)data_field) != RESET)
        {
            flag = 1;
        }
        break;
    // 查询第n条探测器上电记录
    case kPowerOnRecord:
        LOG_I("kPowerOnRecord(%d)", c1);
        data_len = sizeof(TsRecordsTime);
        flag = Flash_GetRecord(kPowerOnRecord, c1, (TsRecordsTime *)data_field);
        break;
    // 查询气体传感器失效记录
    case kSensorFailureRecord:
        LOG_I("kSensorFailureRecord(%d)", c1);
        data_len = 7;
        if(Flash_GetRecord(kSensorFailureRecord, 1, (TsRecordsTime *)&data_field[1]) != RESET)
        {
            // 未失效
            if(data_field[1] == 0)
            {
                // data_field[0] = 0;
                rt_memset(data_field, 0, data_len);
            }
            else
            {
                data_field[0] = 1;
            }
            flag = 1;
        }
        break;
    case kGetCurrentTime:
        // AA 00 08 00 B2 55
        LOG_I("kGetCurrentTime");
        data_len = sizeof(TsRecordsTime);
        RTC_Get_Time();
        data_field[0] = (uint8_t)(RtcDateTime.year >> 8);
        data_field[1] = (uint8_t)(RtcDateTime.year);
        data_field[2] = RtcDateTime.month;
        data_field[3] = RtcDateTime.day;
        data_field[4] = RtcDateTime.hour;
        data_field[5] = RtcDateTime.minute;
        flag = 1;
        break;
    case kGetCurrentTimeSecond:
        // AA 00 09 00 B3 55
        LOG_I("kGetCurrentTimeSecond");
        data_len = 7;
        RTC_Get_Time();
        data_field[0] = (uint8_t)(RtcDateTime.year >> 8);
        data_field[1] = (uint8_t)(RtcDateTime.year);
        data_field[2] = RtcDateTime.month;
        data_field[3] = RtcDateTime.day;
        data_field[4] = RtcDateTime.hour;
        data_field[5] = RtcDateTime.minute;
        data_field[6] = RtcDateTime.second;
        flag = 1;
        break;
    case kSetCurrentTime:
        // AA 00 0A 06 18 03 1D 06 07 08 06 55
        LOG_I("kSetCurrentTime");
        data_len = 7;
        // RTC_Set_Time((uint16_t)(2000 + pHostFrameData->data[0]), pHostFrameData->data[1]
        //     , pHostFrameData->data[2], pHostFrameData->data[3]
        //     , pHostFrameData->data[4], pHostFrameData->data[5]);
        RTC_Set_Time((uint16_t)(pHostFrameData->data[0] << 8 | pHostFrameData->data[1])
            , pHostFrameData->data[2], pHostFrameData->data[3], pHostFrameData->data[4]
            , pHostFrameData->data[5], pHostFrameData->data[6]);
        // SlaveFrameData->data[0] = (uint8_t)(RtcDateTime.year / 256);
        // SlaveFrameData->data[1] = (uint8_t)(RtcDateTime.year % 256);
        // SlaveFrameData->data[2] = RtcDateTime.month;
        // SlaveFrameData->data[3] = RtcDateTime.day;
        // SlaveFrameData->data[4] = RtcDateTime.hour;
        // SlaveFrameData->data[5] = RtcDateTime.minute;
        // SlaveFrameData->data[6] = RtcDateTime.second;
        // Flash_SetProductTimeLimit((uint16_t)(pHostFrameData->data[0] << 8 | pHostFrameData->data[1])
        //     , pHostFrameData->data[2], pHostFrameData->data[3], pHostFrameData->data[4]
        //     , pHostFrameData->data[5], pHostFrameData->data[6], kFactoryTimeId);
        // Set_ExpirationTime(MAX_EXPIRATION_DAYS);
        // Flash_SetProductTimeLimit((uint16_t)(pHostFrameData->data[0] << 8 | pHostFrameData->data[1])
        //     , pHostFrameData->data[2], pHostFrameData->data[3], pHostFrameData->data[4]
        //     , pHostFrameData->data[5], pHostFrameData->data[6], kExpirationTimeId);
        RTC_Get_Time();
        data_field[0] = (uint8_t)(RtcDateTime.year >> 8);
        data_field[1] = (uint8_t)(RtcDateTime.year);
        data_field[2] = RtcDateTime.month;
        data_field[3] = RtcDateTime.day;
        data_field[4] = RtcDateTime.hour;
        data_field[5] = RtcDateTime.minute;
        data_field[6] = RtcDateTime.second;
        flag = 1;
        break;
    case kSetFactoryTime:
        // AA 00 0B 05 17 03 1D 0F 30 2F 55
        LOG_I("kSetFactoryTime");
        data_len = 7;
        Flash_SetProductTimeLimit((uint16_t)(pHostFrameData->data[0] << 8 | pHostFrameData->data[1])
            , pHostFrameData->data[2], pHostFrameData->data[3], pHostFrameData->data[4]
            , pHostFrameData->data[5], pHostFrameData->data[6], kExpirationTimeId);
        Flash_GetProductTimeLimit(&ReadLimitTime, kFactoryTimeId);
        // LOG_D("kSetFactoryTime: %04d-%02d-%02d, %02d:%02d",
        //     ReadLimitTime.Struct.year, ReadLimitTime.Struct.month, ReadLimitTime.Struct.day,
        //     ReadLimitTime.Struct.hour, ReadLimitTime.Struct.minute);
        data_field[0] = (uint8_t)(ReadLimitTime.Struct.year / 256);
        data_field[1] = (uint8_t)(ReadLimitTime.Struct.year % 256);
        data_field[2] = ReadLimitTime.Struct.month;
        data_field[3] = ReadLimitTime.Struct.day;
        data_field[4] = ReadLimitTime.Struct.hour;
        data_field[5] = ReadLimitTime.Struct.minute;
        data_field[6] = ReadLimitTime.Struct.second;
        flag = 1;
        break;
    case kSetExpirationTime:
        // AA 00 0C 05 1C 03 1D 0F 30 35 55
        LOG_I("kSetExpirationTime");
        data_len = 7;
        Flash_SetProductTimeLimit((uint16_t)(pHostFrameData->data[0] << 8 | pHostFrameData->data[1])
            , pHostFrameData->data[2], pHostFrameData->data[3], pHostFrameData->data[4]
            , pHostFrameData->data[5], pHostFrameData->data[6], kExpirationTimeId);
        Flash_GetProductTimeLimit(&ReadLimitTime, kExpirationTimeId);
        // LOG_D("kSetExpirationTime: %04d-%02d-%02d, %02d:%02d",
        //     ReadLimitTime.Struct.year, ReadLimitTime.Struct.month, ReadLimitTime.Struct.day,
        //     ReadLimitTime.Struct.hour, ReadLimitTime.Struct.minute);
        data_field[0] = (uint8_t)(ReadLimitTime.Struct.year / 256);
        data_field[1] = (uint8_t)(ReadLimitTime.Struct.year % 256);
        data_field[2] = ReadLimitTime.Struct.month;
        data_field[3] = ReadLimitTime.Struct.day;
        data_field[4] = ReadLimitTime.Struct.hour;
        data_field[5] = ReadLimitTime.Struct.minute;
        data_field[6] = ReadLimitTime.Struct.second;
        flag = 1;
        break;
    case kSetMqAlarmValue:
        // AA 00 0D 02 0B B8 7B 55
        // LOG_I("设置传感器标定的报警阈值");
        data_len = 2;
        uint16_t value = (uint16_t)(pHostFrameData->data[1] << 8) | pHostFrameData->data[0];
        Flash_SetMQ_AlarmValue(value);
        LOG_D("kSetMqAlarmValue: %d", Flash_GetMQ_AlarmValue());
        data_field[0] = (uint8_t)(value >> 8);
        data_field[1] = (uint8_t)(value);
        flag = 1;
        break;
    case kGetImei:
        data_len = 15;
        // rt_memmove(data_field, DeviceInfo.imei, 15);
        LOG_D("kGetImei:%s", data_field);
        flag = 1;
        break;
    case kGetIccid:
        data_len = 20;
        // memmove(data_field, DeviceInfo.iccid, 20);
        LOG_D("kGetIccid:%s", data_field);
        flag = 1;
        break;
    case kGetIp:
        data_len = 4;
        // memmove(data_field, DeviceInfo.ip, 4);
        LOG_D("kGetIp:%02d.%02d.%02d.%02d"
            , data_field[0], data_field[1]
            , data_field[2], data_field[3]);
        flag = 1;
        break;

    default:
        break;
    }

    if(flag == 1)
    {
        TsRawFrameData RawData;
        HR_GenerateRawFrame(&RawData, c1, c2, data_field, data_len);
        // HR_SendSlaveFrame(SlaveFrameData);
        if (from == kFromUart)
        {
            UART2_Write(RawData.buf, RawData.len);
        }
        else if (from == kFromIot)
        {
            // Air780_Send(RawData.buf, RawData.len);
        }
    }

    return flag;
}

static void hr_thread_entry(void *param)
{
    rt_uint32_t uart2_event_recved;
    uint8_t buf[UART2_RX_RB_LENGTH];
    while (1)
    {
        // rt_sem_take(uart2_rx_ok_sem, RT_WAITING_FOREVER);

        if (RT_EOK == rt_event_recv(uart2_event
						, UART2_EVENT_RX_OK_FLAG
						, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR
						, RT_WAITING_FOREVER
						, &uart2_event_recved))
        {
            if (RT_EOK == rt_event_recv(uart2_event
						, UART2_EVENT_RX_PE_FLAG
						, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR
						, 0
						, &uart2_event_recved))
            {
                LOG_D("UART2_EVENT_RX_PE_FLAG");
                lwrb_skip(&uart2_rx_rb, lwrb_get_full(&uart2_rx_rb));
            }
            else
            {
                LOG_D("UART2_EVENT_RX_OK_FLAG");
                uint8_t buf_len = lwrb_get_full(&uart2_rx_rb);
                lwrb_read(&uart2_rx_rb, buf, buf_len);
                // UART2_Write(buf, buf_len);
                LOG_HEX("u2 buf", 16, buf, buf_len);

            #if 1
                if (buf_len >= HOST_FRAME_MIN_LEN)
                {
                    TsFrameData *HostFrameData = HR_GetFrameData(buf, buf_len);

                    if(HostFrameData != RT_NULL)
                    {
                        // LOG_D("c1: %d, c2: %d, len: %d", HostFrameData->c1, \
                        //     HostFrameData->c2, HostFrameData->len);
                        // logHexDumpAll(HostFrameData->data, HostFrameData->len);

                        HR_ProcessData(HostFrameData, kFromUart);
                    }
                    rt_free(HostFrameData);
                    HostFrameData = RT_NULL;

                    // lwrb_skip(&usart2_rx_rb, buf_len);/* Skip buffer, it has been successfully parse*/
                }
            #endif
            }
        }
    }
}

int BSP_HR_Init(void)
{
    UART2_Init(HR_BAUDRATE, kCheckEven);

    rt_thread_init(&hr_thread,
                   "hr_thread",
                   hr_thread_entry,
                   RT_NULL,
                   &hr_thread_stack[0],
                   sizeof(hr_thread_stack),
                   HR_THREAD_PRIORITY, HR_THREAD_TIMESLICE);
    rt_thread_startup(&hr_thread);

    LOG_I("BSP_HR_Init");
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_APP_EXPORT(BSP_HR_Init);
#endif



