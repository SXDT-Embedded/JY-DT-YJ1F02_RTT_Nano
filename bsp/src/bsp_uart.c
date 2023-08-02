#include "bsp_uart.h"
#include "drv_usart.h"
#include "stdio.h"
#include "board.h"

#define LOG_TAG     "bsp_uart"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

#ifndef RT_USING_SERIAL

static rt_thread_t u2_rx_thread = RT_NULL;

void u2_send(void)
{
    USART2_SendString("USART2_SendString\r\n");
}
MSH_CMD_EXPORT(u2_send, u2_send);


uint8_t buf[USART2_RX_BUFFER_LENGTH];

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
#ifdef RT_USING_COMPONENTS_INIT
INIT_DEVICE_EXPORT(u2_init);
#endif


#if 0
/*********************************************************************
 * @fn      _write
 *
 * @brief   Support Printf Function
 *
 * @param   *buf - UART send Data.
 *          size - Data length
 *
 * @return  size: Data length
 */
__attribute__((used)) int _write(int fd, char *buf, int size)
{
    int i;

    for(i = 0; i < size; i++)
    {
#if(DEBUG == DEBUG_UART1)
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//        USART_SendData(USART1, *buf++);
        USART1_SendArray(buf++, 1);
#elif(DEBUG == DEBUG_UART2)
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, *buf++);
#elif(DEBUG == DEBUG_UART3)
        while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
        USART_SendData(USART3, *buf++);
#endif
    }

    return size;
}

// /*********************************************************************
//  * @fn      _sbrk
//  *
//  * @brief   Change the spatial position of data segment.
//  *
//  * @return  size: Data length
//  */
// void *_sbrk(ptrdiff_t incr)
// {
//     extern char _end[];
//     extern char _heap_end[];
//     static char *curbrk = _end;

//     if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
//     return NULL - 1;

//     curbrk += incr;
//     return curbrk - incr;
// }
#endif

#endif //!RT_USING_SERIAL
