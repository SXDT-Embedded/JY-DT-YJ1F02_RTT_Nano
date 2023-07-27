
#include "ch32v20x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"

#include "bsp_led.h"
#include "bsp_buzzer.h"

#include "drv_usart.h"

#define DBG_TAG     "led"
#define DBG_LVL     DBG_LOG
#include <rtdbg.h>

int main(void)
{
    LOG_I("MCU: CH32V20x");
    LOG_I("SysClk: %dHz",SystemCoreClock);

	while(1)
	{
	    LED_R_TOGGLE();
	    NETLED_R_TOGGLE();
	    rt_thread_mdelay(500);

	    LED_G_TOGGLE();
        NETLED_G_TOGGLE();
        rt_thread_mdelay(500);

        LED_Y_TOGGLE();
        NETLED_Y_TOGGLE();
        rt_thread_mdelay(500);

//        USART1_SendString("main\r\n");
	}
}

