
#include "ch32v20x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"

#include "bsp_led.h"


#define DBG_TAG     "led"
#define DBG_LVL     DBG_LOG
#include <rtdbg.h>

int main(void)
{
    LOG_I("MCU: CH32V20x");
    LOG_I("SysClk: %dHz",SystemCoreClock);

	while(1)
	{
	    LED_G_TOGGLE;
	    rt_thread_mdelay(500);
	}
}

