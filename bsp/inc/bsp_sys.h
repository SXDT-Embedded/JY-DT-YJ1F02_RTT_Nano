/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 15:47:14
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 15:52:22
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\inc\bsp_sys.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_SYS_H__
#define __BSP_SYS_H__

#include "board.h"

// 预热3min
//#define SYS_PREHEAT_TIME_MS      ((uint32_t)(1000 * 60 * 3))
#define SYS_PREHEAT_TIME_MS         ((uint32_t)(1000 * 60 * 3))

#define SYS_MUTE_TIME_MS            ((uint32_t)(1000 * 10))

#define MAX_EXPIRATION_DAYS         ((uint16_t)(365 * 5 + 30 * 3))

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
