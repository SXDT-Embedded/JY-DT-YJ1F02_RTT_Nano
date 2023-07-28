#include "bsp_uart.h"
#include "drv_usart.h"

#define DBG_TAG     "bsp_uart"
#define LOG_TAG     "bsp_uart"
#define LOG_LVL     LOG_LVL_DBG
#include <ulog.h>

static rt_thread_t u2_rx_thread = RT_NULL;

void u2_send()
{
    USART2_SendString("USART2_SendString\r\n");
}
MSH_CMD_EXPORT(u2_send, u2_send);


uint8_t buf[256];

static void u2_rx_thread_entry(void* parameter)
{
    rt_kprintf("u2_rx_thread_entry\r\n");

    while (1)
    {
        rt_sem_take(uart2_revok_sem, RT_WAITING_FOREVER);

        uint8_t buf_len = lwrb_get_full(&usart2_rx_rb);
        lwrb_read(&usart2_rx_rb, buf, buf_len);
        USART2_SendArray(buf, buf_len);
        // LOG_HEX("buf", 16, buf, buf_len);
    }
}

static int u2_init()
{
    u2_rx_thread = rt_thread_create("u2_rx_thread"
                            , u2_rx_thread_entry
                            , RT_NULL
                            , 512
                            , 4
                            , 20);
    if (u2_rx_thread != RT_NULL)
    {
        rt_thread_startup(u2_rx_thread);
        LOG_D("u2_init");
        return RT_EOK;
    }

    return RT_ERROR;
}
INIT_APP_EXPORT(u2_init);
