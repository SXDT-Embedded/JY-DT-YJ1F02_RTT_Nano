#include <rtthread.h>
#include <rthw.h>

#include "bsp_cpu_usage.h"

#define LOG_TAG     "bsp_cpu_usage" // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

// #define CPU_USAGE_CALC_TICK     (1000)
#define CPU_USAGE_CALC_TICK     RT_TICK_PER_SECOND
#define CPU_USAGE_LOOP          (100)

static rt_uint8_t cpu_usage_major, cpu_usage_minor;
static rt_uint32_t total_count;

static void _CPU_Usage_IdleHook(void)
{
    rt_tick_t tick;
    rt_uint32_t count;
    volatile rt_uint32_t loop;

    if (total_count == 0)
    {
        /* get total count */
        rt_enter_critical();
        tick = rt_tick_get();
        while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
        {
            total_count++;
            loop = 0;
            while (loop < CPU_USAGE_LOOP)
                loop++;
        }
        rt_exit_critical();
    }

    count = 0;
    /* get CPU usage */
    tick = rt_tick_get();
    while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
    {
        count++;
        loop = 0;
        while (loop < CPU_USAGE_LOOP)
            loop++;
    }

    /* calculate major and minor */
    if (count < total_count)
    {
        count = total_count - count;
        cpu_usage_major = (count * 100) / total_count;
        cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
    }
    else
    {
        total_count = count;

        /* no CPU usage */
        cpu_usage_major = 0;
        cpu_usage_minor = 0;
    }
}

void CPU_Usage_Get(rt_uint8_t *major, rt_uint8_t *minor)
{
    RT_ASSERT(major != RT_NULL);
    RT_ASSERT(minor != RT_NULL);

    *major = cpu_usage_major;
    *minor = cpu_usage_minor;
}

int CPU_Usage_Init(void)
{
    /* 设置空闲线程钩子函数 */
    return rt_thread_idle_sethook(_CPU_Usage_IdleHook);
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_APP_EXPORT(CPU_Usage_Init);
#endif

//static rt_thread_t get_cpu_use_thread = RT_NULL;
//
//static void get_cpu_use_thread_entry(void *parameter)
//{
//    rt_uint8_t major, minor;
//
//    while (1)
//    {
//        /* 获取CPU利用率数据 */
//        CPU_Usage_Get(&major, &minor);
//
//        /* 打印CPU利用率 */
//        rt_kprintf("CPU利用率 = %d.%d%%\r\n", major, minor);
//
//        rt_thread_delay(1000); /* 延时1000个tick */
//    }
//}

//static void hook_of_scheduler(struct rt_thread* from, struct rt_thread* to)
//{
//    rt_kprintf("from: %s -->  to: %s \n", from->name , to->name);
//}

static int usage(void)
{
    /* 设置调度器钩子 */
//    rt_scheduler_sethook(hook_of_scheduler);
//    cpu_usage_init();

//    get_cpu_use_thread =                           /* 线程控制块指针 */
//        rt_thread_create("get_cpu_use",            /* 线程名字 */
//                         get_cpu_use_thread_entry, /* 线程入口函数 */
//                         RT_NULL,                  /* 线程入口函数参数 */
//                         512,                      /* 线程栈大小 */
//                         5,                        /* 线程的优先级 */
//                         20);                      /* 线程时间片 */
//
//    /* 启动线程，开启调度 */
//    if (get_cpu_use_thread != RT_NULL)
//        rt_thread_startup(get_cpu_use_thread);
//    else
//        return -1;
    rt_uint8_t major, minor;
    /* 获取CPU利用率数据 */
    CPU_Usage_Get(&major, &minor);

    /* 打印CPU利用率 */
    LOG_D("usage = %d.%d%%", major, minor);

    return 0;
}
MSH_CMD_EXPORT(usage, usage);
