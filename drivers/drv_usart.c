
#include "board.h"
#include <rtdevice.h>
#include "drv_usart.h"

//#define DRV_DEBUG
// #define LOG_TAG              "drv.uart"
// #include <drv_log.h>

//#define DRV_DEBUG
#define DBG_TAG              "drv.usart"
#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif /* DRV_DEBUG */
#include <rtdbg.h>

#ifdef RT_USING_SERIAL

#if !defined(BSP_USING_UART1) && !defined(BSP_USING_UART2) && !defined(BSP_USING_UART3) && !defined(BSP_USING_UART4)
    #error "Please define at least one BSP_USING_UARTx"
    /* this driver can be disabled at menuconfig -> RT-Thread Components -> Device Drivers */
#endif

enum
{
#ifdef BSP_USING_UART1
    UART1_INDEX,
#endif
#ifdef BSP_USING_UART2
    UART2_INDEX,
#endif
#ifdef BSP_USING_UART3
    UART3_INDEX,
#endif
#ifdef BSP_USING_UART4
    UART4_INDEX,
#endif
};

static struct ch32_uart_config uart_config[] =
{
#ifdef BSP_USING_UART1
    {
        "uart1",
        USART1,
        USART1_IRQn,
    },
#endif
#ifdef BSP_USING_UART2
    {
        "uart2",
        USART2,
        USART2_IRQn,
    },
#endif
#ifdef BSP_USING_UART3
    {
        "uart3",
        USART3,
        USART3_IRQn,
    },
#endif
#ifdef BSP_USING_UART4
    {
        "uart4",
        UART4,
        UART4_IRQn,
    },
#endif
};

static struct ch32_uart uart_obj[sizeof(uart_config) / sizeof(uart_config[0])] = {0};

static rt_err_t ch32_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct ch32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = (struct ch32_uart *) serial->parent.user_data;

    uart->Init.USART_BaudRate             = cfg->baud_rate;
    uart->Init.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    uart->Init.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;

    switch (cfg->data_bits)
    {
    case DATA_BITS_8:
        uart->Init.USART_WordLength = USART_WordLength_8b;
        break;
    case DATA_BITS_9:
        uart->Init.USART_WordLength = USART_WordLength_9b;
        break;
    default:
        uart->Init.USART_WordLength = USART_WordLength_8b;
        break;
    }

    switch (cfg->stop_bits)
    {
    case STOP_BITS_1:
        uart->Init.USART_StopBits   = USART_StopBits_1;
        break;
    case STOP_BITS_2:
        uart->Init.USART_StopBits   = USART_StopBits_2;
        break;
    default:
        uart->Init.USART_StopBits   = USART_StopBits_1;
        break;
    }
    switch (cfg->parity)
    {
    case PARITY_NONE:
        uart->Init.USART_Parity    = USART_Parity_No;
        break;
    case PARITY_ODD:
        uart->Init.USART_Parity    = USART_Parity_Odd;
        break;
    case PARITY_EVEN:
        uart->Init.USART_Parity    = USART_Parity_Even;
        break;
    default:
        uart->Init.USART_Parity     = USART_Parity_No;
        break;
    }

    if(uart->config->Instance == USART1)
    {
        GPIO_InitTypeDef GPIO_InitStructure = {0};
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

        /* 串口1 GPIO引脚初始化 */
        /* USART1 TX-->A.9   RX-->A.10 */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        USART_Init(uart->config->Instance,&uart->Init);
        USART_Cmd(uart->config->Instance, ENABLE);
    }
    if(uart->config->Instance == USART2)
    {
        GPIO_InitTypeDef GPIO_InitStructure = {0};
        RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, ENABLE);

        /* 串口2 GPIO引脚初始化 */
        /* USART2 TX-->A.2   RX-->A.3 */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        USART_Init(uart->config->Instance,&uart->Init);
        USART_Cmd(uart->config->Instance, ENABLE);
    }
    if(uart->config->Instance == USART3)
    {
        GPIO_InitTypeDef GPIO_InitStructure = {0};
        RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB2Periph_GPIOB, ENABLE);

        /* 串口3 GPIO引脚初始化 */
        /* USART3 TX-->B.10   RX-->B.11 */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        USART_Init(uart->config->Instance,&uart->Init);
        USART_Cmd(uart->config->Instance, ENABLE);
    }
    if(uart->config->Instance == UART4)
    {

    }

    return RT_EOK;
}

static rt_err_t ch32_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct ch32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = (struct ch32_uart *)serial->parent.user_data;
    switch (cmd)
    {
    /* disable interrupt */
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
        NVIC_DisableIRQ(uart->config->irq_type);
        /* disable interrupt */
        USART_ITConfig(uart->config->Instance, USART_IT_RXNE, DISABLE);
        break;
    /* enable interrupt */
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
        NVIC_EnableIRQ(uart->config->irq_type);
        /* enable interrupt */
        USART_ITConfig(uart->config->Instance, USART_IT_RXNE, ENABLE);
        break;
    }
    return RT_EOK;
}

static int ch32_putc(struct rt_serial_device *serial, char c)
{
    struct ch32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = (struct ch32_uart *)serial->parent.user_data;
    while (USART_GetFlagStatus(uart->config->Instance, USART_FLAG_TC) == RESET);
    uart->config->Instance->DATAR = c;
    return 1;
}

static int ch32_getc(struct rt_serial_device *serial)
{
    int ch;
    struct ch32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = (struct ch32_uart *)serial->parent.user_data;
    ch = -1;
    if (USART_GetFlagStatus(uart->config->Instance, USART_FLAG_RXNE) != RESET)
    {
        ch = uart->config->Instance->DATAR & 0xff;
    }
    return ch;
}

rt_size_t ch32dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
    return RT_EOK;
}

static void uart_isr(struct rt_serial_device *serial)
{
    struct ch32_uart *uart = (struct ch32_uart *) serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);
    if (USART_GetITStatus(uart->config->Instance, USART_IT_RXNE) != RESET)
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
        USART_ClearITPendingBit(uart->config->Instance, USART_IT_RXNE);
    }
}

static const struct rt_uart_ops ch32_uart_ops =
{
    ch32_configure,
    ch32_control,
    ch32_putc,
    ch32_getc,
    ch32dma_transmit
};


#ifdef BSP_USING_UART1
void USART1_IRQHandler(void) __attribute__((interrupt()));
void USART1_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART1_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif

#ifdef BSP_USING_UART2
void USART2_IRQHandler(void) __attribute__((interrupt()));
void USART2_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART2_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif

#ifdef BSP_USING_UART3
void USART3_IRQHandler(void) __attribute__((interrupt()));
void USART3_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART3_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif

#ifdef BSP_USING_UART4
void UART4_IRQHandler(void) __attribute__((interrupt()));
void UART4_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART4_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif

int rt_hw_usart_init(void)
{
    rt_size_t obj_num = sizeof(uart_obj) / sizeof(struct ch32_uart);
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    rt_err_t result = 0;

    for (int i = 0; i < obj_num; i++)
    {
        if (uart_obj[i].config->name == "uart1")
        {
            /* init UART object */
            uart_obj[i].config        = &uart_config[i];
            uart_obj[i].serial.ops    = &ch32_uart_ops;
            uart_obj[i].serial.config = config;

            /* register UART device */
            result = rt_hw_serial_register(&uart_obj[i].serial, uart_obj[i].config->name,
                                        RT_DEVICE_FLAG_RDWR
                                        | RT_DEVICE_FLAG_INT_RX
                                        , &uart_obj[i]);
            RT_ASSERT(result == RT_EOK);
        }
        // TODO: 准备测试下 串口 DMA收发
        else
        {
            /* init UART object */
            uart_obj[i].config        = &uart_config[i];
            uart_obj[i].serial.ops    = &ch32_uart_ops;
            uart_obj[i].serial.config = config;

            /* register UART device */
            result = rt_hw_serial_register(&uart_obj[i].serial, uart_obj[i].config->name,
                                        RT_DEVICE_FLAG_RDWR
                                        | RT_DEVICE_FLAG_DMA_RX
                                        | RT_DEVICE_FLAG_DMA_TX
                                        , &uart_obj[i]);
            RT_ASSERT(result == RT_EOK);
        }
    }

    return result;
}
#else   /* RT_USING_SERIAL */
#include <string.h>
#include "lwutil.h"

// USART RX buffer for DMA to transfer every received byte
// Contains raw data that are about to be processed by different events
uint8_t usart1_rx_dma_buffer[USART1_RX_BUFFER_LENGTH] = {0};
uint8_t usart2_rx_dma_buffer[USART2_RX_BUFFER_LENGTH] = {0};
uint8_t usart3_rx_dma_buffer[USART3_RX_BUFFER_LENGTH] = {0};

lwrb_t usart1_rx_rb;
// Ring buffer data array for RX DMA
uint8_t usart1_rx_rb_data[USART1_RX_BUFFER_LENGTH];

// Ring buffer instance for RX data
lwrb_t usart2_rx_rb;
// Ring buffer data array for RX DMA
uint8_t usart2_rx_rb_data[USART2_RX_BUFFER_LENGTH];

lwrb_t usart3_rx_rb;
// Ring buffer data array for RX DMA
uint8_t usart3_rx_rb_data[USART3_RX_BUFFER_LENGTH];

//  Ring buffer instance for TX data
lwrb_t usart1_tx_rb;
lwrb_t usart2_tx_rb;
lwrb_t usart3_tx_rb;

// Ring buffer data array for TX DMA
uint8_t usart1_tx_rb_data[USART1_TX_RB_LENGTH];
uint8_t usart2_tx_rb_data[USART2_TX_RB_LENGTH];
uint8_t usart3_tx_rb_data[USART3_TX_RB_LENGTH];

// Length of currently active TX DMA transfer
volatile size_t usart1_tx_dma_current_len;
volatile size_t usart2_tx_dma_current_len;
volatile size_t usart3_tx_dma_current_len;

volatile uint8_t uart1_rev_parity_flag = 0; //  串口1接收数据奇偶校验的标志
volatile uint8_t uart2_rev_parity_flag = 0; //  串口2接收数据奇偶校验的标志
volatile uint8_t uart3_rev_parity_flag = 0; //  串口3接收数据奇偶校验的标志

volatile uint8_t uart1_rev_flag = 0; //  串口2接收到数据待解析的标志
volatile uint8_t uart2_rev_flag = 0; //  串口2接收到数据待解析的标志
volatile uint8_t uart3_rev_flag = 0; //  串口3接收到数据待解析的标志

void USART1_IRQHandler(void) __attribute__((interrupt()));
void USART2_IRQHandler(void) __attribute__((interrupt()));
void USART3_IRQHandler(void) __attribute__((interrupt()));
// void USART4_IRQHandler(void) __attribute__((interrupt()));

// USART1 - TX
void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt()));

// USART2 - RX
void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt()));
// USART2 - TX
void DMA1_Channel7_IRQHandler(void) __attribute__((interrupt()));

// USART3 - RX
void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt()));
// USART3 - TX
void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt()));

void USART1_RxCheck(void);
void USART1_ProcessData(const void* data, size_t len);

void USART2_RxCheck(void);
void USART2_ProcessData(const void* data, size_t len);
void USART2_SendString(const char* str);
uint8_t USART2_StartTxDMATransfer(void);

void USART3_RxCheck(void);
void USART3_ProcessData(const void* data, size_t len);
void USART3_SendString(const char* str);
uint8_t USART3_StartTxDMATransfer(void);

void USART_SendByte(USART_TypeDef *pUSARTx, const uint16_t data)
{
	/* 发送一个字节数据到USART */
    USART_SendData(pUSARTx, data);

    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void USART_SendString(USART_TypeDef *pUSARTx, const char *str)
{
    uint16_t i = 0;
    do {
            USART_SendByte( pUSARTx, *(str + i) );
            i++;
        } while (*(str + i)!='\0');
    /* 等待发送完成 */
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

void USART_SendArray(USART_TypeDef *pUSARTx, const uint8_t *pdata, uint16_t len)
{
    while (len--)
    {
         USART_SendByte( pUSARTx, *(pdata++) );
    }
    /* 等待发送完成 */
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

void USART1_RxCheck(void)
{
    static size_t old_pos;
    size_t pos;

    if(uart1_rev_parity_flag == 0)
    {
        /* Calculate current position in buffer and check for new data available */
        pos = LWUTIL_ARRAYSIZE(usart1_rx_dma_buffer) - DMA_GetCurrDataCounter(USART1_DMA_RX_CHANNEL);

        /* Check change in received data */
        if (pos != old_pos)
        {
            /* Current position is over previous one */
            if (pos > old_pos)
            {
                USART1_ProcessData(&usart1_rx_dma_buffer[old_pos], pos - old_pos);
            }
            else
            {
                USART1_ProcessData(&usart1_rx_dma_buffer[old_pos], LWUTIL_ARRAYSIZE(usart1_rx_dma_buffer) - old_pos);
                if (pos > 0)
                {
                    USART1_ProcessData(&usart1_rx_dma_buffer[0], pos);
                }
            }
            old_pos = pos;  /* Save current position as old for next transfers */
        }
    }
    else
    {
        pos = LWUTIL_ARRAYSIZE(usart1_rx_dma_buffer) - DMA_GetCurrDataCounter(USART1_DMA_RX_CHANNEL);
        old_pos = pos;  /* Save current position as old for next transfers */
        uart1_rev_parity_flag = 0;
    }
}

void USART2_RxCheck(void)
{
    static size_t old_pos;
    size_t pos;

    if(uart2_rev_parity_flag == 0)
    {
        /* Calculate current position in buffer and check for new data available */
        pos = LWUTIL_ARRAYSIZE(usart2_rx_dma_buffer) - DMA_GetCurrDataCounter(USART2_DMA_RX_CHANNEL);

        /* Check change in received data */
        if (pos != old_pos)
        {
            /* Current position is over previous one */
            if (pos > old_pos)
            {
                USART2_ProcessData(&usart2_rx_dma_buffer[old_pos], pos - old_pos);
            }
            else
            {
                USART2_ProcessData(&usart2_rx_dma_buffer[old_pos], LWUTIL_ARRAYSIZE(usart2_rx_dma_buffer) - old_pos);
                if (pos > 0)
                {
                    USART2_ProcessData(&usart2_rx_dma_buffer[0], pos);
                }
            }
            old_pos = pos;  /* Save current position as old for next transfers */
        }
    }
    else
    {
        pos = LWUTIL_ARRAYSIZE(usart2_rx_dma_buffer) - DMA_GetCurrDataCounter(USART2_DMA_RX_CHANNEL);
        old_pos = pos;  /* Save current position as old for next transfers */
        uart2_rev_parity_flag = 0;
    }
}

void USART3_RxCheck(void)
{
    static size_t old_pos;
    size_t pos;

    if(uart3_rev_parity_flag == 0)
    {
        /* Calculate current position in buffer and check for new data available */
        pos = LWUTIL_ARRAYSIZE(usart3_rx_dma_buffer) - DMA_GetCurrDataCounter(USART3_DMA_RX_CHANNEL);

        /* Check change in received data */
        if (pos != old_pos)
        {
            /* Current position is over previous one */
            if (pos > old_pos)
            {
                USART3_ProcessData(&usart3_rx_dma_buffer[old_pos], pos - old_pos);
            }
            else
            {
                USART3_ProcessData(&usart3_rx_dma_buffer[old_pos], LWUTIL_ARRAYSIZE(usart3_rx_dma_buffer) - old_pos);
                if (pos > 0)
                {
                    USART3_ProcessData(&usart3_rx_dma_buffer[0], pos);
                }
            }
            old_pos = pos;  /* Save current position as old for next transfers */
        }
    }
    else
    {
        pos = LWUTIL_ARRAYSIZE(usart3_rx_dma_buffer) - DMA_GetCurrDataCounter(USART3_DMA_RX_CHANNEL);
        old_pos = pos;  /* Save current position as old for next transfers */
        uart3_rev_parity_flag = 0;
    }
}

/**
 * \brief       Check if DMA is active and if not try to send data
 * \return      `1` if transfer just started, `0` if on-going or no data to transmit
 */
uint8_t USART1_StartTxDMATransfer(void)
{
    uint8_t started = 0;
    rt_enter_critical();

    if (usart1_tx_dma_current_len == 0
        && (usart1_tx_dma_current_len = lwrb_get_linear_block_read_length(&usart1_tx_rb)) > 0)
    {
        /* Limit maximal size to transmit at a time */
        // if (usart1_tx_dma_current_len > 256)
        // {
        //     usart1_tx_dma_current_len = 256;
        // }

        /* Disable channel if enabled */
        DMA_Cmd(USART1_DMA_TX_CHANNEL, DISABLE);

        /* Clear all flags */
        DMA_ClearFlag(DMA1_FLAG_TC4);
        DMA_ClearFlag(DMA1_FLAG_HT4);
        DMA_ClearFlag(DMA1_FLAG_GL4);
        DMA_ClearFlag(DMA1_FLAG_TE4);

        USART1_DMA_TX_CHANNEL->CNTR = usart1_tx_dma_current_len;
        USART1_DMA_TX_CHANNEL->MADDR = (uint32_t)lwrb_get_linear_block_read_address(&usart1_tx_rb);

        /* Start transfer */
        DMA_Cmd(USART1_DMA_TX_CHANNEL, ENABLE);
        // logDebug("USART1_StartTxDMATransfer");

        started = 1;
    }
    rt_exit_critical();

    return started;
}

/**
 * \brief       Check if DMA is active and if not try to send data
 * \return      `1` if transfer just started, `0` if on-going or no data to transmit
 */
uint8_t USART2_StartTxDMATransfer(void)
{
    uint8_t started = 0;
    rt_enter_critical();

    if (usart2_tx_dma_current_len == 0
        && (usart2_tx_dma_current_len = lwrb_get_linear_block_read_length(&usart2_tx_rb)) > 0)
    {
        /* Disable channel if enabled */
        DMA_Cmd(USART2_DMA_TX_CHANNEL, DISABLE);

        /* Clear all flags */
        DMA_ClearFlag(DMA1_FLAG_TC7);
        DMA_ClearFlag(DMA1_FLAG_HT7);
        DMA_ClearFlag(DMA1_FLAG_GL7);
        DMA_ClearFlag(DMA1_FLAG_TE7);

        USART2_DMA_TX_CHANNEL->CNTR = usart2_tx_dma_current_len;
        USART2_DMA_TX_CHANNEL->MADDR = (uint32_t)lwrb_get_linear_block_read_address(&usart2_tx_rb);

        /* Start transfer */
        DMA_Cmd(USART2_DMA_TX_CHANNEL, ENABLE);
        // logDebug("USART2_StartTxDMATransfer");

        started = 1;
    }
    rt_exit_critical();

    return started;
}

uint8_t USART3_StartTxDMATransfer(void)
{
    uint8_t started = 0;

    rt_enter_critical();

    if (usart3_tx_dma_current_len == 0
        && (usart3_tx_dma_current_len = lwrb_get_linear_block_read_length(&usart3_tx_rb)) > 0)
    {
        /* Disable channel if enabled */
        DMA_Cmd(USART3_DMA_TX_CHANNEL, DISABLE);

        /* Clear all flags */
        DMA_ClearFlag(DMA1_FLAG_TC2);
        DMA_ClearFlag(DMA1_FLAG_HT2);
        DMA_ClearFlag(DMA1_FLAG_GL2);
        DMA_ClearFlag(DMA1_FLAG_TE2);

        USART3_DMA_TX_CHANNEL->CNTR = usart3_tx_dma_current_len;
        USART3_DMA_TX_CHANNEL->MADDR = (uint32_t)lwrb_get_linear_block_read_address(&usart3_tx_rb);

        /* Start transfer */
        DMA_Cmd(USART3_DMA_TX_CHANNEL, ENABLE);
        // logDebug("USART2_StartTxDMATransfer");

        started = 1;
    }

    rt_exit_critical();

    return started;
}


void USART1_ProcessData(const void* data, size_t len)
{
    lwrb_write(&usart1_rx_rb, data, len);  /* Write data to receive buffer */
}

/**
 * \brief           Process received data over UART2
 * Data are written to RX ringbuffer for application processing at latter stage
 * \param[in]       data: Data to process
 * \param[in]       len: Length in units of bytes
 */
void USART2_ProcessData(const void* data, size_t len)
{
    lwrb_write(&usart2_rx_rb, data, len);
}

void USART3_ProcessData(const void* data, size_t len)
{
    lwrb_write(&usart3_rx_rb, data, len);
}

/**
 * \brief           Send string to USART1
 * \param[in]       str: String to send
 */
void USART1_SendString(const char* str)
{
    lwrb_write(&usart1_tx_rb, str, strlen(str)); /* Write data to TX buffer for loopback */
    USART1_StartTxDMATransfer();              /* Then try to start transfer */
}

/**
 * \brief           Send Array to USART1
 * \param[in]       str: Array to send
 */
void USART1_SendArray(const void* data, size_t len)
{
    lwrb_write(&usart1_tx_rb, data, len);
    USART1_StartTxDMATransfer();
}

/**
 * \brief           Send string to USART2
 * \param[in]       str: String to send
 */
void USART2_SendString(const char* str)
{
    lwrb_write(&usart2_tx_rb, str, strlen(str)); /* Write data to TX buffer for loopback */
    USART2_StartTxDMATransfer();              /* Then try to start transfer */
}

/**
 * \brief           Send Array to USART2
 * \param[in]       str: Array to send
 */
void USART2_SendArray(const void* data, size_t len)
{
    lwrb_write(&usart2_tx_rb, data, len); /* Write data to TX buffer for loopback */
    USART2_StartTxDMATransfer();              /* Then try to start transfer */
}

void USART3_SendString(const char* str)
{
    lwrb_write(&usart3_tx_rb, str, strlen(str)); /* Write data to TX buffer for loopback */
    USART3_StartTxDMATransfer();              /* Then try to start transfer */
}

unsigned int USART3_SendArray(const void* data, unsigned int len)
{
    unsigned int ret;
    ret = lwrb_write(&usart3_tx_rb, (uint8_t *)data, len); /* Write data to TX buffer for loopback */
    USART3_StartTxDMATransfer();              /* Then try to start transfer */
    return ret;
}

unsigned int UART3_Read(void *buf, unsigned int len)
{
    return lwrb_read(&usart3_rx_rb, buf, len);
}

void USART1_Init(uint32_t baudrate, TeUsartParityMode parity)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};			// DMA 配置

    /* Initialize ringbuff */
    lwrb_init(&usart1_rx_rb, usart1_rx_rb_data, sizeof(usart1_rx_rb_data));
    lwrb_init(&usart1_tx_rb, usart1_tx_rb_data, sizeof(usart1_tx_rb_data));

    USART_DeInit(USART1);   // 寄存器恢复默认值

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* 串口1 GPIO引脚初始化 */
    /* USART1 TX-->A.9   RX-->A.10 */
    GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = USART1_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);

    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_PE, ENABLE);

    /* 串口DMA配置 */
#if 1
    /* UART1 RX DMA init */
    DMA_DeInit(USART1_DMA_RX_CHANNEL);  // DMA1 通道5,寄存器复位, uart1_rx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart1_rx_dma_buffer; // 接收内存地址，从哪里接收
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(usart1_rx_dma_buffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(USART1_DMA_RX_CHANNEL, &DMA_InitStructure);
#endif
    // /* UART1 TX DMA init */
    DMA_DeInit(USART1_DMA_TX_CHANNEL);  // DMA1 通道4,寄存器复位, uart1_tx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart1_tx_rb_data;  //传输再修改地址，并使能该通道
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(usart1_tx_rb_data);
    // DMA_InitStructure.DMA_BufferSize = (uint32_t)0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(USART1_DMA_TX_CHANNEL, &DMA_InitStructure);

    /* Enable HT & TC interrupts for RX */
    DMA_ITConfig(USART1_DMA_RX_CHANNEL, DMA_IT_HT, ENABLE);
    DMA_ITConfig(USART1_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);

    /* Enable TC interrupts for TX */
    DMA_ITConfig(USART1_DMA_TX_CHANNEL, DMA_IT_TC, ENABLE);

    /* DMA interrupt init for RX & TX */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_DMA_TX_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_DMA_RX_IRQ_CHANNEL;
    NVIC_Init(&NVIC_InitStructure);

    /* Initialize UART1 */
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    switch (parity)
    {
    case kCheck0Stop1:
        break;
    case kCheck0Stop2:
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
        break;
    case kCheckOdd:
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        break;
    case kCheckEven:
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        break;
    default:
        break;
    }
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_PE, ENABLE);

    /* UART interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable UART and DMA */
    DMA_Cmd(USART1_DMA_RX_CHANNEL, ENABLE);
    DMA_Cmd(USART1_DMA_TX_CHANNEL, DISABLE);

    USART_Cmd(USART1, ENABLE);

    LOG_D("USART1 Init");
}

void USART2_Init(uint32_t baudrate, TeUsartParityMode parity)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};			// DMA 配置

    /* Initialize ringbuff for TX & RX */
    lwrb_init(&usart2_rx_rb, usart2_rx_rb_data, sizeof(usart2_rx_rb_data));
    lwrb_init(&usart2_tx_rb, usart2_tx_rb_data, sizeof(usart2_tx_rb_data));
    // lwrb_set_evt_fn(&uart2_rx_fifo, USART2_BufferEvent);

    USART_DeInit(USART2);   // 寄存器恢复默认值

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* 串口2 GPIO引脚初始化 */
    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = USART2_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);

    // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_PE, ENABLE);

    /* 串口DMA配置 */
    /* UART2 RX DMA init */
    DMA_DeInit(USART2_DMA_RX_CHANNEL);  // DMA1 通道6,寄存器复位, uart2_rx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DATAR); /* USART2->DATAR:0x40004404 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_rx_dma_buffer; // 接收内存地址，从哪里接收
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(usart2_rx_dma_buffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(USART2_DMA_RX_CHANNEL, &DMA_InitStructure);

    // /* UART2 TX DMA init */
    DMA_DeInit(USART2_DMA_TX_CHANNEL);  // DMA1 通道7,寄存器复位, uart2_tx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DATAR); /* USART2->DATAR:0x40004404 */
    // DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_tx_rb_data;  //传输再修改地址，并使能该通道
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    // DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(usart2_tx_rb_data);
    // DMA_InitStructure.DMA_BufferSize = (uint32_t)0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(USART2_DMA_TX_CHANNEL, &DMA_InitStructure);

    /* Enable HT & TC interrupts for RX */
    DMA_ITConfig(USART2_DMA_RX_CHANNEL, DMA_IT_HT, ENABLE);
    DMA_ITConfig(USART2_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);

    /* Enable TC interrupts for TX */
    DMA_ITConfig(USART2_DMA_TX_CHANNEL, DMA_IT_TC, ENABLE);

    /* DMA interrupt init for RX & TX */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_DMA_RX_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_DMA_TX_IRQ_CHANNEL;
    NVIC_Init(&NVIC_InitStructure);

    /* Initialize UART2 */
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    switch (parity)
    {
    case kCheck0Stop1:
        break;
    case kCheck0Stop2:
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
        break;
    case kCheckOdd:
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        break;
    case kCheckEven:
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        break;
    default:
        break;
    }
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStructure);

    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART2, USART_IT_PE, ENABLE);

    /* UART interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable UART and DMA */
    DMA_Cmd(USART2_DMA_RX_CHANNEL, ENABLE);
    DMA_Cmd(USART2_DMA_TX_CHANNEL, DISABLE);

    USART_Cmd(USART2, ENABLE);

    LOG_D("USART2 Init");
}

void USART3_Init(uint32_t baudrate, TeUsartParityMode parity)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};			// DMA 配置

    /* Initialize ringbuff for TX & RX */
    lwrb_init(&usart3_rx_rb, usart3_rx_rb_data, sizeof(usart3_rx_rb_data));
    lwrb_init(&usart3_tx_rb, usart3_tx_rb_data, sizeof(usart3_tx_rb_data));
    // lwrb_set_evt_fn(&uart2_rx_fifo, USART2_BufferEvent);

    USART_DeInit(USART3);   // 寄存器恢复默认值

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);

    /* 串口3 GPIO引脚初始化 */
    /* USART3 TX-->B.10   RX-->B.11 */
    GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = USART3_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStructure);

    // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_PE, ENABLE);

    /* 串口DMA配置 */
    /* UART2 RX DMA init */
    DMA_DeInit(USART3_DMA_RX_CHANNEL);  // DMA1 通道6,寄存器复位, uart2_rx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DATAR); /* USART2->DATAR:0x40004404 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart3_rx_dma_buffer; // 接收内存地址，从哪里接收
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(usart3_rx_dma_buffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(USART3_DMA_RX_CHANNEL, &DMA_InitStructure);

    // /* UART2 TX DMA init */
    DMA_DeInit(USART3_DMA_TX_CHANNEL);  // DMA1 通道7,寄存器复位, uart3_tx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DATAR); /* USART3->DATAR:0x40004404 */
    // DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart3_tx_rb_data;  //传输再修改地址，并使能该通道
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    // DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(usart3_tx_rb_data);
    // DMA_InitStructure.DMA_BufferSize = (uint32_t)0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(USART3_DMA_TX_CHANNEL, &DMA_InitStructure);

    /* Enable HT & TC interrupts for RX */
    DMA_ITConfig(USART3_DMA_RX_CHANNEL, DMA_IT_HT, ENABLE);
    DMA_ITConfig(USART3_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);

    /* Enable TC interrupts for TX */
    DMA_ITConfig(USART3_DMA_TX_CHANNEL, DMA_IT_TC, ENABLE);

    /* DMA interrupt init for RX & TX */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_DMA_RX_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_DMA_TX_IRQ_CHANNEL;
    NVIC_Init(&NVIC_InitStructure);

    /* Initialize UART2 */
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    switch (parity)
    {
    case kCheck0Stop1:
        break;
    case kCheck0Stop2:
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
        break;
    case kCheckOdd:
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        break;
    case kCheckEven:
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        break;
    default:
        break;
    }
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStructure);

    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);

    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART3, USART_IT_PE, ENABLE);

    /* UART interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable UART and DMA */
    DMA_Cmd(USART3_DMA_RX_CHANNEL, ENABLE);
    DMA_Cmd(USART3_DMA_TX_CHANNEL, DISABLE);

    USART_Cmd(USART3, ENABLE);

    LOG_D("USART3 Init");
}

/**
 * @description: DMA1 channel2 interrupt handler for UART3 TX
 * @return {*}
 */
void DMA1_Channel2_IRQHandler(void)
{
    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC2) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC2); /* Clear transfer complete flag */
        lwrb_skip(&usart3_tx_rb, usart3_tx_dma_current_len);/* Skip buffer, it has been successfully sent out */
        usart3_tx_dma_current_len = 0;           /* Reset data length */
        USART3_StartTxDMATransfer();          /* Start new transfer */
    }

    /* Implement other events when needed */
}

/**
 * @description: DMA1 channel3 interrupt handler for UART3 RX
 * @return {*}
 */
void DMA1_Channel3_IRQHandler(void)
{
    /* Check half-transfer complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_HT3) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_HT3); /* Clear half-transfer complete flag */
        USART3_RxCheck();                       /* Check data */
    }
    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC3) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC3); /* Clear transfer complete flag */
        USART3_RxCheck();                       /* Check data */
    }

    /* Implement other events when needed */
}

/**
 * @description: DMA1 channel4 interrupt handler for UART1 TX
 * @return {*}
 */
void DMA1_Channel4_IRQHandler(void)
{
    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC4) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC4); /* Clear transfer complete flag */
        lwrb_skip(&usart1_tx_rb, usart1_tx_dma_current_len);/* Skip buffer, it has been successfully sent out */
        usart1_tx_dma_current_len = 0;           /* Reset data length */
        USART1_StartTxDMATransfer();          /* Start new transfer */
    }

    /* Implement other events when needed */
}

/**
 * @description: DMA1 channel6 interrupt handler for UART2 RX
 * @return {*}
 */
void DMA1_Channel6_IRQHandler(void)
{
    /* Check half-transfer complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_HT6) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_HT6); /* Clear half-transfer complete flag */
        USART2_RxCheck();                       /* Check data */
    }
    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC6) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC6); /* Clear transfer complete flag */
        USART2_RxCheck();                       /* Check data */
    }

    /* Implement other events when needed */
}

/**
 * @description: DMA1 channel7 interrupt handler for UART2 TX
 * @return {*}
 */
void DMA1_Channel7_IRQHandler(void)
{
    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC7) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC7); /* Clear transfer complete flag */
        lwrb_skip(&usart2_tx_rb, usart2_tx_dma_current_len);/* Skip buffer, it has been successfully sent out */
        usart2_tx_dma_current_len = 0;           /* Reset data length */
        USART2_StartTxDMATransfer();          /* Start new transfer */
    }

    /* Implement other events when needed */
}

void USART1_IRQHandler(void)
{
    uint8_t temp = 0;

    if(USART_GetITStatus(USART1, USART_IT_PE) != RESET) //校验错误
    {
        uart1_rev_parity_flag = 1;
    }
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//接收完数据后进入空闲中断
    {
        // 如果IDLEIE已经被置位，则会产生对应的中断。读状态寄存器再读数据寄存器的操作会清除此位
  	    temp = USART1->STATR;
        temp = USART1->DATAR;

            USART1_RxCheck();
    }
    /* Implement other events when needed */

    temp &= 0;
}

/**
 * @description: UART2 global interrupt handler
 * @return {*}
 */
void USART2_IRQHandler(void)
{
    uint8_t temp = 0;

    if(USART_GetITStatus(USART2, USART_IT_PE) != RESET)//校验错误
    {
        uart2_rev_parity_flag = 1;
    }
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)//接收完数据后进入空闲中断
    {
        // 如果IDLEIE已经被置位，则会产生对应的中断。读状态寄存器再读数据寄存器的操作会清除此位
  	    temp = USART2->STATR;
        temp = USART2->DATAR;

            USART2_RxCheck();
        uart2_rev_flag = 1; //检测到空闲状态，置位接收完成位
    }
    /* Implement other events when needed */

    temp &= 0;
}

void USART3_IRQHandler(void)
{
    uint8_t temp = 0;

    if(USART_GetITStatus(USART3, USART_IT_PE) != RESET)//校验错误
    {
        uart3_rev_parity_flag = 1; //校验错误
    }
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)//接收完数据后进入空闲中断
    {
        // 如果IDLEIE已经被置位，则会产生对应的中断。读状态寄存器再读数据寄存器的操作会清除此位
  	    temp = USART3->STATR;
        temp = USART3->DATAR;

        USART3_RxCheck();
    }
    /* Implement other events when needed */

    temp &= 0;
}

int rt_hw_usart_init(void)
{
    USART1_Init(115200, kCheck0Stop1);
    USART2_Init(115200, kCheck0Stop1);
    USART3_Init(115200, kCheck0Stop1);

    return 0;
}




#endif /* RT_USING_SERIAL */

