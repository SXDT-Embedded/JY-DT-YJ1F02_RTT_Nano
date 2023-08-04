/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 15:48:22
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 16:39:07
 * @FilePath     : \JT-DT-YD1F01_RTT_Nano\bsp\src\bsp_sys.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_sys.h"

#define LOG_TAG     "bsp_sys"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

//用到了atoi
#include <stdlib.h>

TsSysControl SysControl;

void SYS_SetStatus(TeSysStatus sys_status)
{
    SysControl.status = sys_status;
    LOG_D("SysControl.status = %d", SysControl.status);
}

static void SET_SYS_Status(int argc, char **argv)
{
    if (argc == 2)
    {
        SYS_SetStatus((TeSysStatus)atoi(argv[1]));
    }
    else
    {
        LOG_E("--used SET_SYS_Status [status]");
        return;
    }
}
MSH_CMD_EXPORT(SET_SYS_Status, "status");

void BSP_SYS_Init(void)
{
    SYS_SetStatus(kSysPowerUpStatus);
}
