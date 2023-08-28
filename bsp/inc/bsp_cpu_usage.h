/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-01 13:20:11
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-04 16:34:26
 * @FilePath     : \JY-DT-YJ1F02_RTT_Nano\bsp\inc\bsp_cpu_usage.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_CPU_USAGE_H__
#define __BSP_CPU_USAGE_H__

#include <rtthread.h>
#include <rthw.h>


/* 获取cpu利用率 */
int CPU_Usage_Init(void);
void CPU_Usage_Get(rt_uint8_t *major, rt_uint8_t *minor);

#endif  // !__BSP_CPU_USAGE_H__
