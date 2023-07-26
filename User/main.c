
#include "ch32v20x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"

#include "bsp_led.h"
#include "bsp_buzzer.h"

#define DBG_TAG     "led"
#define DBG_LVL     DBG_LOG
#include <rtdbg.h>

int main(void)
{
    LOG_I("MCU: CH32V20x");
    LOG_I("SysClk: %dHz",SystemCoreClock);

	while(1)
	{
//	    LED_G_TOGGLE;
//	    rt_thread_mdelay(500);
//	    BUZZER_TOGGLE;
	    //    rt_hw_us_delay(500);
	    //    rt_thread_mdelay(10);
	    //    rt_hw_us_delay(500);
	    //    uint32_t i = 0;
	    //    for (i = 0; i < 1000; i++)
	    //    {
	    //
	    //    }
	    //    rt_hw_us_delay(900);
//        rt_thread_mdelay(1);

//	    BUZZER_TOGGLE;
//	    rt_hw_us_delay(1 * 1000);
//	    BUZZER_TOGGLE;
	    rt_thread_mdelay(1);
	}
}

