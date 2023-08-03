#include "ch32v20x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"

#include "bsp_led.h"
#include "bsp_buzzer.h"
#include "bsp_mq.h"

#define LOG_TAG     "main"
#define LOG_LVL     LOG_LVL_DBG
#include <ulog.h>

int main(void)
{
    LOG_I("MCU: CH32V20x");
    LOG_I("SysClk: %dHz",SystemCoreClock);

	rt_uint32_t mq_recved;

	while(1)
	{
		if (RT_EOK == rt_event_recv(&mq_event, MQ_EVENT_ALARM_FLAG | MQ_EVENT_END_OF_LIFE_FLAG
						| MQ_EVENT_OPEN_CIRCUIT_FLAG | MQ_EVENT_SHORT_CIRCUIT_FLAG
						, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO
						, &mq_recved))
		{
			switch (mq_recved)
			{
			case MQ_EVENT_ALARM_FLAG:
				LOG_D("rev MQ_EVENT_ALARM_FLAG");
				break;
			case MQ_EVENT_END_OF_LIFE_FLAG:
				LOG_D("rev MQ_EVENT_END_OF_LIFE_FLAG");
				break;
			case MQ_EVENT_OPEN_CIRCUIT_FLAG:
				LOG_D("rev MQ_EVENT_OPEN_CIRCUIT_FLAG");
				break;
			case MQ_EVENT_SHORT_CIRCUIT_FLAG:
				LOG_D("rev MQ_EVENT_SHORT_CIRCUIT_FLAG");
				break;

			default:
				break;
			}
		}

	    rt_thread_mdelay(1);
		// LOG_D("test USING_RT_KPRINTF_THREADSAFE");
    }
}
