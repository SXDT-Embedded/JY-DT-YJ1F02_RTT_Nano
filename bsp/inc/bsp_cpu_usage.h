/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-01 13:20:11
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-01 13:20:11
 * @FilePath     : \JT-DT-YD1F01_RTT_Nano\bsp\inc\bsp_cpu_usage.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_CPU_USAGE_H__
#define __BSP_CPU_USAGE_H__

#include <rtthread.h>
#include <rthw.h>


/* 获取cpu利用率 */
int cpu_usage_init();
void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor);
int usage(void);

#endif  // !__BSP_CPU_USAGE_H__
