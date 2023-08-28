/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 11:49:34
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 17:23:04
 * @FilePath     : \JY-DT-YJ1F02_RTT_Nano\bsp\inc\bsp_mq.h
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

// #define MQ_ALARM_DEFAULT_LEL        (10)

#define MQ_LIFE_TIMS_YEAR           (3)
#define MQ_END_OF_LIFE_TIMS_S       ((uint32_t)(3600 * 24 * 365 * MQ_LIFE_TIMS_YEAR))

#define MQ_THREAD_STACK_SIZE    (512)
#define MQ_THREAD_PRIORITY      9
#define MQ_THREAD_TIMESLICE     5

#define MQ_EVENT_NORMAL_FLAG            (1 << 0)
#define MQ_EVENT_ALARM_FLAG             (1 << 1)
#define MQ_EVENT_END_OF_LIFE_FLAG       (1 << 2)
#define MQ_EVENT_OPEN_CIRCUIT_FLAG      (1 << 3)
#define MQ_EVENT_SHORT_CIRCUIT_FLAG     (1 << 4)
#define MQ_EVENT_CALIBRATION_FLAG       (1 << 5)
#define MQ_EVENT_FAULT_FLAG             (1 << 6)

#define MQ_EVENT_TO_BE_CALIB_FLAG       (1 << 7)

extern struct rt_event mq_event;

typedef struct __attribute__((packed))
{
    uint32_t expiration_seconds;
    uint16_t alarm_value;

    // uint16_t valve_buffer[MQ_SAMPLING_TIMS];
    uint8_t status_buffer[MQ_SAMPLING_TIMS];

    uint8_t detection_flag;
} TeMq;
extern TeMq Mq;


int BSP_MQ_Init(void);
uint16_t Get_MQ_VoltageInt1000x(void);



#endif // !__BSP_MQ_H__
