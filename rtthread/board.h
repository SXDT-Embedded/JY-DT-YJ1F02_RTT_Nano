/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-09-22     Bernard      add board.h to this bsp
 * 2017-10-20     ZYH          emmm...setup for HAL Libraries
 */

// <<< Use Configuration Wizard in Context Menu >>>
#ifndef __BOARD_H__
#define __BOARD_H__

#include "ch32v20x.h"

#define ch32v20x_PIN_NUMBERS   48

/* board configuration */
//#define SRAM_SIZE  20
//#define SRAM_END (0x20000000 + SRAM_SIZE * 1024)    // ������ַ = 0x20000000����ַ�� + 20K(RAM��С)

//extern int __stack_size;
//extern int _ebss;
//#define HEAP_BEGIN  ((void *)&_ebss)
//#define HEAP_END    (void *)(SRAM_END - (int)&__stack_size)

extern int _ebss, _heap_end;
#define HEAP_BEGIN  ((void *)&_ebss)
#define HEAP_END    ((void *)&_heap_end)

void rt_hw_board_init(void);

#endif /* __BOARD_H__ */
