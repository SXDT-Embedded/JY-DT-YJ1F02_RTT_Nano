/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 15:47:14
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-07 10:49:14
 * @FilePath     : \JY-DT-YJ1F02_RTT_Nano\bsp\inc\bsp_sys.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_SYS_H__
#define __BSP_SYS_H__

#include "board.h"
#include <rtthread.h>
#include <rthw.h>

// 预热3min
//#define SYS_PREHEAT_TIME_MS      ((uint32_t)(1000 * 60 * 3))
#define SYS_PREHEAT_TIME_MS         ((uint32_t)(1000 * 60 * 3))

#define SYS_MUTE_TIME_MS            ((uint32_t)(1000 * 10))

#define MAX_EXPIRATION_DAYS         ((uint16_t)(365 * 5 + 30 * 3))

typedef struct __attribute__((packed))
{
    uint16_t preheat_time_s;        // 开机预热时长(S)
    uint16_t mute_time_s;           // 按键消音时长(S)

    /************气体传感器 MQ-4B************************/
    // uint16_t mq_expiration_days;        // 气体传感器 寿命天数
    // adc voltage 1000x
    uint16_t mq_alarm_voltage;          // 气体传感器 报警 电压阈值
    uint16_t mq_open_circuit_voltage;   // 气体传感器 开路 电压阈值
    uint16_t mq_short_circuit_voltage;  // 气体传感器 短路 电压阈值

    uint8_t factory_time[6];        // 出厂时间 年-月-日-时-分-秒
    uint8_t expiration_time[6];     // 到期时间 年-月-日-时-分-秒
    uint16_t expiration_seconds;    // 到期时间 (s)

} TsSysCtrl;
extern TsSysCtrl SysCtrl;

/* sys 事件控制块 */
extern rt_event_t sys_event;

#define EVENT_NORMAL_FLAG               (1 << 0)
#define EVENT_ALARM_FLAG                (1 << 1)
#define EVENT_END_OF_LIFE_FLAG          (1 << 2)
#define EVENT_OPEN_CIRCUIT_FLAG         (1 << 3)
#define EVENT_SHORT_CIRCUIT_FLAG        (1 << 4)
#define EVENT_CALIBRATION_FLAG          (1 << 5)
#define EVENT_FAULT_FLAG                (1 << 6)

#define EVENT_BUTTON_SINGLE_CLICK       (1 << 7)
#define EVENT_BUTTON_DOUBLE_CLICK       (1 << 8)
#define EVENT_BUTTON_LONG_CLICK         (1 << 9)

#define EVENT_BUTTON_LONG_CLICK         (1 << 9)


typedef enum
{
    // kSysProductionModeStatus = 0,   // 出厂前的生产模式
    // kSysUserModeStatus,             // 用户模式

    kSysToBeCalibStatus = 0,        // 初始化系统待标定状态
    kSysGasCalibStatus,             // 气体标定状态
    KSysTestStatus,                 // 通气测试态

    kSysSelfCheckingStatus,         // 自检

    kSysPowerUpStatus,              // 上电状态
    kSysPowerDownStatus,            // 掉电状态

    kSysStartupPreheatingStatus,    // 开机预热状态
    kSysRuningStatus,               // 正常运行状态

    kSysAlarmStatus,                // 报警状态
    kSysMuteStatus,                  // 消音状态

    kSysFaultStatus,                // 故障状态
    kSysSensorEndOfLifeStatus,      // 传感器寿命到期状态

    kSysRestoreFactoryStatus,       // 恢复出厂设置状态
}TeSysStatus;

typedef struct
{
    uint8_t alarm;
    uint8_t alarm_rcy;
    uint8_t fault;
    uint8_t fault_rcy;
    uint8_t power_failure;
    uint8_t power_on;
    uint8_t mq_failure;
    uint8_t mute;
    uint8_t preheat;

    uint8_t self_check_led_off;
    uint8_t self_check_action_output;
    uint8_t self_check_action_rcy;

    uint8_t actuator;
    uint8_t mq_detection;
    uint8_t action_rcy;

    uint8_t preheating_to_self_check;

    uint8_t production_user_mode;   // 0:production mode
    uint8_t mq_to_be_calib;
} TsSysFlag;

typedef enum
{
    kSysProductionMode = 1,
    kSysUserMode,
} TeSysRunMode;

typedef struct
{
    TeSysStatus status;
    TeSysStatus last_status;
    TsSysFlag Flag;
    // TeMqStatus mq_status;
}TsSysControl;

extern TsSysControl SysControl;


void BSP_SYS_Init(void);
void SYS_SetStatus(TeSysStatus sys_status);
// void SYS_Loop(void);








#endif // !__BSP_SYS_H__
