#include "ch32v20x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"

#include "bsp_led.h"
#include "bsp_buzzer.h"

#define LOG_TAG     "main"
#define LOG_LVL     LOG_LVL_DBG
#include <ulog.h>

int main(void)
{
    LOG_I("MCU: CH32V20x");
    LOG_I("SysClk: %dHz",SystemCoreClock);

	while(1)
	{
//        for (int i = 0; i < 10000; i++);
//
//	    LED_R_TOGGLE();
//	    NETLED_R_TOGGLE();
	    rt_thread_mdelay(2);
    }
}
