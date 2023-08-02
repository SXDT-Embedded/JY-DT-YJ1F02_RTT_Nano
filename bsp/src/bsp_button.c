/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 17:15:25
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 17:32:10
 * @FilePath     : \JT-DT-YD1C01_RTT_Nano\bsp\src\bsp_button.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_button.h"
#include "multi_button.h"

#include "bsp_led.h"
#include "bsp_buzzer.h"
#include "bsp_emv.h"
#include "bsp_sys.h"

#define LOG_TAG     "bsp_button"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

ALIGN(RT_ALIGN_SIZE)
static char button_thread_stack[BUTTON_THREAD_STACK_SIZE];
static struct rt_thread button_thread;

//先申请一个按键结构
struct Button button_mute_check;

/**
 * @description: 按键读取GPIO电平
 * @param {TeButtonId} button_id   自己定义的按键id
 * @return {uint8_t}            0-引脚低电平，(1)-引脚高电平
 */
static uint8_t Button_ReadPin(uint8_t button_id)
{
    switch (button_id)
    {
    case kButtonMuteCheckId:
        return READ_BUTTON_MUTE_CHECK;
        break;

    default:
        return -1;
        break;
    }
}

static void ButtonMuteCheck_SingleClickCb(void *btn)
{
    LOG_D("ButtonMuteCheck SingleClick!");

    // if (SysControl.status == kSysRuningStatus ||
    //     SysControl.status == kSysStartupPreheatingStatus)
    // {
    //     SYS_SetStatus(kSysSelfCheckingStatus);
    // }
}

static void ButtonMuteCheck_DoubleClickCb(void *btn)
{
    LOG_D("ButtonMuteCheck DoubleClick");
    // Coil_Close();
}

/**
 * @description: 消音按键长按开始回调函数
 * @param {void*} btn
 * @return {void}
 */
static void ButtonMuteCheck_LongStartCb(void *btn)
{
    LOG_D("ButtonMuteCheck LongStart");

    // if (SysControl.status == kSysAlarmStatus)
    // {
    //     SYS_SetStatus(kSysMuteStatus);
    // }
    // else if (SysControl.status == kSysRuningStatus)
    // {
    //     SysControl.Flag.action_rcy = 1;
    // }
}

static void button_thread_entry(void *param)
{
    while (1)
    {
        //每隔5ms调用一次按键后台处理函数
        button_ticks();
        rt_thread_mdelay(5);
    }
}

/**
 * @description: 按键初始化
 * @return {*}
 */
int BSP_BUTTON_Init(void)
{
    rt_pin_mode(BUTTON_MUTE_CHECK_PIN, PIN_MODE_INPUT_PULLUP);

    button_init(&button_mute_check, Button_ReadPin, 0, kButtonMuteCheckId);

    // 注册button_mute的事件回调函数
    button_attach(&button_mute_check, SINGLE_CLICK, ButtonMuteCheck_SingleClickCb);
    button_attach(&button_mute_check, DOUBLE_CLICK, ButtonMuteCheck_DoubleClickCb);
    button_attach(&button_mute_check, LONG_PRESS_START, ButtonMuteCheck_LongStartCb);

    // 启动按键
    button_start(&button_mute_check);

    rt_thread_init(&button_thread,
                   "button_thread",
                   button_thread_entry,
                   RT_NULL,
                   &button_thread_stack[0],
                   sizeof(button_thread_stack),
                   BUTTON_THREAD_PRIORITY, BUTTON_THREAD_TIMESLICE);
    rt_thread_startup(&button_thread);

    LOG_I("BSP_BUTTON_Init!");
}
INIT_APP_EXPORT(BSP_BUTTON_Init);

