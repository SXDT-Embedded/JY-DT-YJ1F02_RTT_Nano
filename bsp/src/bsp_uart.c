#include "bsp_uart.h"
#include "drv_usart.h"

#define DBG_TAG     "bsp_uart"
#define DBG_LVL     DBG_LOG
#include <rtdbg.h>

static rt_thread_t u2_rx_thread = RT_NULL;

void u2_send()
{
    USART2_SendString("USART2_SendString\r\n");
}
MSH_CMD_EXPORT(u2_send, u2_send);

static void u2_rx_thread_entry(void* parameter)
{
    rt_kprintf("u2_rx_thread_entry\r\n");

    while (1)
    {
        rt_sem_take(uart2_revok_sem, RT_WAITING_FOREVER);
        // if (rt_sem_take(uart1_rev_sem, RT_WAITING_FOREVER))
        // {
        //     USART1_RxCheck();
        // }
        LOG_D("uart2_revok_sem");
        // USART2_RxCheck();
        // rt_thread_mdelay(10);

        uint8_t buf[256];
        uint8_t buf_len = lwrb_get_full(&usart2_rx_rb);
        lwrb_read(&usart2_rx_rb, buf, buf_len);

        LOG_D("%s", buf);
    }
}

static int u2_init()
{
    u2_rx_thread = rt_thread_create("u2_rx_thread"
                            , u2_rx_thread_entry
                            , RT_NULL
                            , 1024
                            , 5
                            , 20);
    if (u2_rx_thread != RT_NULL)
    {
        rt_thread_startup(u2_rx_thread);
        rt_kprintf("u2_init\r\n");
        return RT_EOK;
    }

    return RT_ERROR;
}
INIT_APP_EXPORT(u2_init);
