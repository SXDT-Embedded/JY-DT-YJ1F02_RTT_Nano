/* RT-Thread config file */

#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

#include <rtthread.h>

// #define RT_USING_FINSH
//当系统加入 FinSH 组件源码后，需要在 rtconfig.h 中开启以下项
#include "finsh_config.h"

#define RT_DEBUG
#define RT_DEBUG_COLOR

//基础配置
// <<< Use Configuration Wizard in Context Menu >>>
// <h>Basic Configuration
// <o>Maximal level of thread priority <8-256>
//  <i>Default: 32
//1、设置系统最大优先级，可设置范围 8 到 256，默认值 32，可修改。
#define RT_THREAD_PRIORITY_MAX  32
// <o>OS tick per second
//  <i>Default: 1000   (1ms)
//2、设置 RT-Thread 操作系统节拍，表示多少 tick 每秒，如默认值为 100 ，表示一个时钟节拍（os tick）长度为 10ms。常用值为 100 或 1000。时钟节拍率越快，系统的额外开销就越大。
#define RT_TICK_PER_SECOND  1000
// <o>Alignment size for CPU architecture data access
//  <i>Default: 4
//3、字节对齐时设定对齐的字节个数，默认 4，常使用 ALIGN (RT_ALIGN_SIZE) 进行字节对齐。
#define RT_ALIGN_SIZE   4
// <o>the max length of object name<2-16>
//  <i>Default: 8
//4、设置对象名称的最大长度，默认 8 个字符，一般无需修改。
#define RT_NAME_MAX    8
// <c1>Using RT-Thread components initialization
//  <i>Using RT-Thread components initialization
//5、设置使用组件自动初始化功能，默认需要使用，开启该宏则可以使用自动初始化功能。
#define RT_USING_COMPONENTS_INIT
// </c>
//6、开启 RT_USING_USER_MAIN 宏，则打开 user_main 功能，默认需要开启，这样才能调用 RT-Thread 的启动代码；main 线程的栈大小可修改。
#define RT_USING_USER_MAIN
// <o>the stack size of main thread<1-4086>
//  <i>Default: 512
#define RT_MAIN_THREAD_STACK_SIZE     512

//内核调试功能配置
//定义 RT_DEBUG 宏则开启 debug 模式。若开启系统调试，则在实现打印之后可以打印系统 LOG 日志。请在代码开发与调试过程中打开该项，帮助调试定位问题，在代码发布时关闭该项。
//#define RT_DEBUG                 // 关闭 debug
#define RT_DEBUG_INIT 0            // 启用组件初始化调试配置，设置为 1 则会打印自动初始化的函数名称
//#define RT_USING_OVERFLOW_CHECK  // 关闭栈溢出检查


//钩子函数配置
//设置是否使用钩子函数，默认关闭。
#define RT_USING_HOOK                         // 是否 开启系统钩子功能
#define RT_USING_IDLE_HOOK                    // 是否 开启空闲线程钩子功能

//软件定时器配置
//设置是否启用软件定时器，以及相关参数的配置，默认关闭。
#define RT_USING_TIMER_SOFT       0             // 关闭软件定时器功能，为 1 则打开
#if RT_USING_TIMER_SOFT == 0
#undef RT_USING_TIMER_SOFT
#endif
#define RT_TIMER_THREAD_PRIO        4           // 设置软件定时器线程的优先级，默认为 4
#define RT_TIMER_THREAD_STACK_SIZE  512         // 设置软件定时器线程的栈大小，默认为 512 字节

//IPC 配置
#define RT_USING_SEMAPHORE       // 设置是否使用 信号量，默认打开
#define RT_USING_MUTEX           // 设置是否使用 互斥量
#define RT_USING_EVENT           // 设置是否使用 事件集
#define RT_USING_MAILBOX         // 设置是否使用  邮箱
#define RT_USING_MESSAGEQUEUE    // 设置是否使用 消息队列

//内存配置
//#define RT_USING_MEMPOOL      // 是否使用 内存池
#define RT_USING_HEAP           // 是否使用 内存堆
#define RT_USING_SMALL_MEM      // 是否使用 小内存管理
//#define RT_USING_TINY_SIZE    // 是否使用 小体积的算法，牵扯到 rt_memset、rt_memcpy 所产生的体积

// <h>Console Configuration
// <c1>Using console
//  <i>Using console
#define RT_USING_CONSOLE
// </c>
// <o>the buffer size of console <1-1024>
//  <i>the buffer size of console
//  <i>Default: 128  (128Byte)
#define RT_CONSOLEBUF_SIZE          128
#define RT_CONSOLE_DEVICE_NAME      "uart1"
// </h>

#if 0
#if defined(RT_USING_FINSH)
    #define FINSH_USING_MSH
    #define FINSH_USING_MSH_ONLY
    // <h>Finsh Configuration
    // <o>the priority of finsh thread <1-7>
    //  <i>the priority of finsh thread
    //  <i>Default: 6
    #define __FINSH_THREAD_PRIORITY     5
    #define FINSH_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX / 8 * __FINSH_THREAD_PRIORITY + 1)
    // <o>the stack of finsh thread <1-4096>
    //  <i>the stack of finsh thread
    //  <i>Default: 4096  (4096Byte)
    #define FINSH_THREAD_STACK_SIZE     1024
    // <o>the history lines of finsh thread <1-32>
    //  <i>the history lines of finsh thread
    //  <i>Default: 5
    #define FINSH_USING_HISTORY
    #define FINSH_HISTORY_LINES         5
    #define FINSH_USING_SYMTAB
    #define FINSH_USING_DESCRIPTION
    #define FINSH_CMD_SIZE 80
    // </h>
#endif
#endif

// <<< end of configuration section >>>

#define RT_USING_SERIAL
#define RT_USING_PIN
#define RT_USING_UART1
#define RT_USING_DEVICE_IPC

//DEVICE 框架配置
//当系统中加入 device 框架源码时，则需要在 rtconfig.h 中开启以下项
#define RT_USING_DEVICE

//#define RT_USING_POSIX

// 使用了 rtdbg log打印
#define DBG_ENABLE

#endif
