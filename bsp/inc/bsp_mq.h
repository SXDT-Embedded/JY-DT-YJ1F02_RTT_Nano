/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 11:49:34
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 11:49:34
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\inc\bsp_mq.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_MQ_H__
#define __BSP_MQ_H__

#include "board.h"

#define MQ_SAMPLING_MS              (500U)
#define MQ_SAMPLING_TIMS            (20U)

// < MQ_SAMPLING_TIMS
// #define MQ_THRESHOLD_NORMAL_TIMS    (12U)
#define MQ_THRESHOLD_ALARM_TIMS     (12U)
#define MQ_THRESHOLD_FAULT_TIMS     (10U)

#define MQ_VOLTAGE_RATIO            (1.5F)

// adc voltage 1000x
#define MQ_VOLTAGE_HIGH_LIMIT       (4850U)
#define MQ_VOLTAGE_LOW_LIMIT        (50U)

#define MQ_VOLTAGE_ALARM_DEFAULT    (uint16_t)(3250)
// 甲烷 5   VOL% = 50,000 ppm = 100 LEL;  1 LEL = 500 ppm
// 丙烷 2.2 VOL% = 22,000 ppm = 100 %LEL; 1 LEL = 220 ppm; 8 LEL = 1760 ppm; 10 LEL = 2200 ppm;

#define MQ_ALARM_DEFAULT_LEL        (10)

#define MQ_LIFE_TIMS_YEAR           (3)
#define MQ_END_OF_LIFE_TIMS_S       ((uint32_t)(3600 * 24 * 365 * MQ_LIFE_TIMS_YEAR))
// 预热3min
// #define MQ_WARM_UP_TIME_MS          ((uint64_t)(1000 * 60 * 3))

// extern uint16_t mq_alarm_value;
// extern uint32_t mq_expiration_seconds;

typedef enum
{
    kMqNormal = 0,
    kMqAlarm,
    kMqEndOfLife,
    kMqOpenCircuit,
    kMqShortCircuit,
    kMqCalibration,
    kMqFault
}TeMqStatus;


typedef struct __attribute__((packed))
{
    uint32_t expiration_seconds;
    uint16_t alarm_value;

    // uint16_t valve_buffer[MQ_SAMPLING_TIMS];
    uint8_t status_buffer[MQ_SAMPLING_TIMS];

    uint8_t detection_flag;
} TeMq;


extern TeMq Mq;


void BSP_MQ_Init(void);

FlagStatus IS_MQ_EndOfLife(void);
TeMqStatus MQ_Detection(void);
TeMqStatus BSP_MQ_Detection(void);

uint16_t Get_MQ_VoltageInt1000x(void);

void BSP_MQ_Loop(void);


#endif // !__BSP_MQ_H__
