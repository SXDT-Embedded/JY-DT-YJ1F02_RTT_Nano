/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 15:23:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-03 10:12:27
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\inc\bsp_history_reading.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_HISTORY_READING_H__
#define __BSP_HISTORY_READING_H__

#include "board.h"

#define HR_THREAD_STACK_SIZE    (1024)
#define HR_THREAD_PRIORITY      (10)
#define HR_THREAD_TIMESLICE     (5)

#define HR_BAUDRATE (4800)

/*************可燃气体探测器报警历史记录读取串口通信协议****************/
// 帧起始符(1)  控制码(2)       数据域长度(1)  数据域(n)    校验码(1)   结束符(1)
// 0xAA        C1、C2(低四位)      L          DATA         CS          0x55
#define FRAME_HEADER    (0xAA)
#define FRAME_TAIL      (0x55)

#define HOST_FRAME_MIN_LEN   (6U)

#define HR_ALARM_MAX_NUM              (200)
#define HR_ALARM_RCY_MAX_NUM          (200)
#define HR_FAULT_MAX_NUM              (100)
#define HR_FAULT_RCY_MAX_NUM          (100)
#define HR_POWER_FAILURE_MAX_NUM      (50)
#define HR_POWER_ON_MAX_NUM           (50)
#define HR_SENSOR_FAILURE_MAX_NUM     (1)

typedef enum
{
    kNumOfRecords = 0,
    kAlarmRecord,           // at least 200
    kAlarmRcyRecord,        // at least 200
    kFaultRecord,           // at least 100
    kFaultRcyRecord,        // at least 100
    kPowerFailureRecord,    // at least 50
    kPowerOnRecord,         // at least 50
    kSensorFailureRecord,   // at least 1
    kGetCurrentTime,

    // 以下是在国标原基础上所扩展的
    kGetCurrentTimeSecond,  // include second

    kSetCurrentTime,
    kSetFactoryTime,
    kSetExpirationTime,

    kSetMqAlarmValue,

    kGetImei,
    kGetIccid,
    kGetIp,

    kUpSelfCheck,       // 上传自检状态
    kUpStartupPreheat,  // 上传开机预热状态
    kUpRun,             // 上传正常运行态
    kUpAlarm,       // 上传报警状态
    kUpMute,        // 上传消音状态
    kUpFault,       // 上传故障状态
    kUpSensorEnd,   // 上传传感器到期状态

} TeFrameC2;

typedef enum
{
    kFromUart = 0,
    kFromIot,
} TeDataSources;
typedef struct __attribute__((packed))
{
    uint8_t year_h;
    uint8_t year_l;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
} TsRecordsTime;

typedef struct __attribute__((packed))
{
    uint8_t c1;
    TeFrameC2 c2;
    uint8_t len;
    uint8_t data[];
} TsFrameData;

typedef struct __attribute__((packed))
{
    uint8_t len;
    uint8_t buf[256];
} TsRawFrameData;

int BSP_HR_Init(void);

void HR_Loop(void);
TsFrameData* HR_GetFrameData(const uint8_t *p_src, const uint8_t src_len);
// FlagStatus HR_ProcessData(const TsFrameData *pHostFrameData);
// uint8_t HR_ProcessData(const TsFrameData *pHostFrameData);
uint8_t HR_ProcessData(const TsFrameData *pHostFrameData, TeDataSources from);

uint8_t HR_GenerateRawFrame(TsRawFrameData* pRawData
    , uint8_t c1,TeFrameC2 c2, const uint8_t* p_src, uint8_t src_len);


#endif // !__BSP_HISTORY_READING_H__
