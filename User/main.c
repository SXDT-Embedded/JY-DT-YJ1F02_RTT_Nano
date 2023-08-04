#include "bsp_sys.h"

#include "bsp_led.h"
// #include "agile_led.h"
#include "bsp_buzzer.h"
#include "bsp_mq.h"

#define LOG_TAG     "main"
#define LOG_LVL     LOG_LVL_DBG
#include <ulog.h>

/* sys 事件控制块 */
rt_event_t sys_event;

int main(void)
{
	rt_uint32_t sys_event_recved;

    LOG_I("MCU: CH32V20x");
    LOG_I("SysClk: %dHz",SystemCoreClock);
	LOG_I("基础款家用可燃气体探测器（甲烷）JT-DT-YD1F01");

    /* 初始化 sys 事件对象 */
    sys_event = rt_event_create("sys_event", RT_IPC_FLAG_PRIO);
    if (sys_event == RT_NULL)
    {
        LOG_D("creat sys_event failed");
    }

	while(1)
	{
		if (RT_EOK == rt_event_recv(sys_event
						, EVENT_ALARM_FLAG | EVENT_END_OF_LIFE_FLAG
						| EVENT_OPEN_CIRCUIT_FLAG | EVENT_SHORT_CIRCUIT_FLAG
						, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR
						, 5
						, &sys_event_recved))
		{
			switch (sys_event_recved)
			{
			case EVENT_ALARM_FLAG:
				LOG_D("rev EVENT_ALARM_FLAG");
				break;

			// 传感器寿命到期失效
			// TODO: 到期后报警状态的切换
			case EVENT_END_OF_LIFE_FLAG:
				LOG_D("rev EVENT_END_OF_LIFE_FLAG");
				agile_led_set_light_mode(led_y, "500,500", -1);
				agile_led_start(led_y);
				break;

			case EVENT_OPEN_CIRCUIT_FLAG:
				LOG_D("rev EVENT_OPEN_CIRCUIT_FLAG");
				break;

			case EVENT_SHORT_CIRCUIT_FLAG:
				LOG_D("rev EVENT_SHORT_CIRCUIT_FLAG");
				break;

			default:
				break;
			}
		}
	    // rt_thread_mdelay(1);
		// LOG_D("test USING_RT_KPRINTF_THREADSAFE");
    }
}

static int TEST_EVENT_END_OF_LIFE_FLAG(void)
{
	return rt_event_send(sys_event, EVENT_END_OF_LIFE_FLAG);
}
MSH_CMD_EXPORT(TEST_EVENT_END_OF_LIFE_FLAG, TEST_EVENT_END_OF_LIFE_FLAG);
