#include "board.h"
#include <rtdevice.h>
#include "drv_uart.h"

#define LOG_TAG     "drv_usart"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

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
        // TODO: serial下的串口 DMA收发待完成
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
// #include <string.h>
#include "lwutil.h"

#if 0
#include "drv_uart_config.h"

typedef struct {
    /* OS queue */
    rt_mq_t queue;                   /*!< Message queue */

    /* Raw data buffer */
    uint8_t dma_rx_buffer[64];                  /*!< DMA buffer for receive data */
    size_t old_pos;                             /*!< Position for data */
} uart_desc_volatile_t;

typedef struct
{
    /* DMA config & flags management */
    DMA_Channel_TypeDef *DmaChannel;  /*!< TX/RX DMA channel */

    uint32_t dma_flag_gl;    // DMAx Channelx global flag
    uint32_t dma_flag_tc;    // DMAx Channelxtransfer complete flag.
    uint32_t dma_flag_ht;    // DMAx Channelx half transfer flag.
    uint32_t dma_flag_te;    // DMAx Channelx transfer error flag.
    // void (*DMA_ClearFlag)(uint32_t DMAy_FLAG);      // Clears the DMAy Channelx's pending flags.

    uint32_t dma_it_gl;    // DMAx Channelx global flag
    uint32_t dma_it_tc;    // DMAx Channelxtransfer complete flag.
    uint32_t dma_it_ht;    // DMAx Channelx half transfer flag.
    uint32_t dma_it_te;    // DMAx Channelx transfer error flag.
    // void (*DMA_ClearITPendingBit)(uint32_t DMAy_IT); // Clears the DMAy Channelx's interrupt pending bits.

    IRQn_Type dma_irq;                          /*!< DMA IRQ instance */
} TsCh32DmaConfig;

typedef struct {
    /* UART config */
    USART_TypeDef *UartInstance;                        /*!< UART/USART/LPUART instance */
    IRQn_Type uart_irq;                         /*!< UART IRQ instance */

    GPIO_TypeDef *uart_tx_port;
    GPIO_TypeDef *uart_rx_port;
    uint16_t uart_tx_pin;
    uint16_t uart_rx_pin;
    uint32_t gpio_remap;                // GPIO_Remap_USART1
    FunctionalState gpio_remap_state;   // ENABLE or DISABLE.
    // Changes the mapping of the specified pin.
    // void (*GPIO_PinRemapConfig)(uint32_t GPIO_Remap, FunctionalState NewState);

    TsCh32DmaConfig dma_tx;
    TsCh32DmaConfig dma_rx;

    /* Interrupts config */
    uint8_t prio;                               /*!< Preemption priority number */

    uart_desc_volatile_t* data;                 /*!< Pointer to volatile data */
} TsCh32Uart;

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

static struct TsCh32Uart ch32_uart_config[] =
{
#ifdef BSP_USING_UART1
    UART1_CONFIG,
#endif
#ifdef BSP_USING_UART2
    UART2_CONFIG,
#endif
#ifdef BSP_USING_UART3
    UART3_CONFIG,
#endif
#ifdef BSP_USING_UART4
    UART4_CONFIG,
#endif
#ifdef BSP_USING_UART5
    UART5_CONFIG,
#endif
#ifdef BSP_USING_UART6
    UART6_CONFIG,
#endif
#ifdef BSP_USING_UART7
    UART7_CONFIG,
#endif
#ifdef BSP_USING_UART8
    UART8_CONFIG,
#endif
#ifdef BSP_USING_LPUART1
    LPUART1_CONFIG,
#endif
};
#endif

// USART RX buffer for DMA to transfer every received byte
// Contains raw data that are about to be processed by different events
static uint8_t _uart1_rx_dma_buffer[UART1_RX_BUFFER_LENGTH] = {0};
static uint8_t _uart2_rx_dma_buffer[UART2_RX_BUFFER_LENGTH] = {0};
static uint8_t _uart3_rx_dma_buffer[UART3_RX_BUFFER_LENGTH] = {0};

//  Ring buffer instance for TX data
lwrb_t uart1_tx_rb;
// Ring buffer data array for TX DMA
uint8_t uart1_tx_rb_data[UART1_TX_RB_LENGTH];
// Ring buffer instance for RX data
lwrb_t uart1_rx_rb;
// Ring buffer data array for RX DMA
uint8_t uart1_rx_rb_data[UART1_RX_RB_LENGTH];

lwrb_t uart2_tx_rb;
uint8_t uart2_tx_rb_data[UART2_TX_RB_LENGTH];
lwrb_t uart2_rx_rb;
uint8_t uart2_rx_rb_data[UART2_RX_RB_LENGTH];

lwrb_t uart3_tx_rb;
uint8_t uart3_tx_rb_data[UART3_TX_RB_LENGTH];
lwrb_t uart3_rx_rb;
uint8_t uart3_rx_rb_data[UART2_RX_RB_LENGTH];

// Length of currently active TX DMA transfer
volatile size_t _uart1_tx_dma_current_len;
volatile size_t _uart2_tx_dma_current_len;
volatile size_t _uart3_tx_dma_current_len;

// struct rt_event uart1_event;
// struct rt_event uart2_event;
// struct rt_event uart3_event;

ALIGN(RT_ALIGN_SIZE)
static char uart1_dma_rx_thread_stack[UART1_DMA_RX_THREAD_STACK_SIZE];
static struct rt_thread uart1_dma_rx_thread;

ALIGN(RT_ALIGN_SIZE)
static char uart2_dma_rx_thread_stack[UART2_DMA_RX_THREAD_STACK_SIZE];
static struct rt_thread uart2_dma_rx_thread;

ALIGN(RT_ALIGN_SIZE)
static char uart3_dma_rx_thread_stack[UART3_DMA_RX_THREAD_STACK_SIZE];
static struct rt_thread uart3_dma_rx_thread;

rt_sem_t uart1_rx_check_sem = RT_NULL;
rt_sem_t uart1_rx_ok_sem = RT_NULL;
rt_sem_t uart1_rx_parity_err_sem = RT_NULL;

rt_sem_t uart2_rx_check_sem = RT_NULL;
rt_sem_t uart2_rx_ok_sem = RT_NULL;
rt_sem_t uart2_rx_parity_err_sem = RT_NULL;

rt_sem_t uart3_rx_check_sem = RT_NULL;
rt_sem_t uart3_rx_ok_sem = RT_NULL;
rt_sem_t uart3_rx_parity_err_sem = RT_NULL;

void USART1_IRQHandler(void) __attribute__((interrupt()));
void USART2_IRQHandler(void) __attribute__((interrupt()));
void USART3_IRQHandler(void) __attribute__((interrupt()));
//void UART4_IRQHandler(void) __attribute__((interrupt()));

// USART1 - TX
void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt()));
// USART1 - RX
void DMA1_Channel5_IRQHandler(void) __attribute__((interrupt()));

// USART2 - RX
void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt()));
// USART2 - TX
void DMA1_Channel7_IRQHandler(void) __attribute__((interrupt()));

// USART3 - RX
void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt()));
// USART3 - TX
void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt()));

// 阻塞式发送的
#ifdef UART_USE_POLLING_TX
void UART_SendByte(USART_TypeDef *pUSARTx, const uint16_t data)
{
	/* 发送一个字节数据到USART */
    USART_SendData(pUSARTx, data);
    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void UART_SendString(USART_TypeDef *pUSARTx, const char *str)
{
    uint16_t i = 0;
    do {
            UART_SendByte( pUSARTx, *(str + i) );
            i++;
        } while (*(str + i)!='\0');
    /* 等待发送完成 */
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

void UART_SendArray(USART_TypeDef *pUSARTx, const uint8_t *pdata, uint16_t len)
{
    while (len--)
    {
         UART_SendByte( pUSARTx, *(pdata++) );
    }
    /* 等待发送完成 */
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}
#endif // !UART_USE_POLLING_TX

/**
 * \brief           Process received data over UART1
 * Data are written to RX ringbuffer for application processing at latter stage
 * \param[in]       data: Data to process
 * \param[in]       len: Length in units of bytes
 */
rt_inline void _UART1_ProcessData(const void* data, size_t len)
{
    /* Write data to receive buffer */
    lwrb_write(&uart1_rx_rb, data, len);
}

rt_inline void _UART2_ProcessData(const void* data, size_t len)
{
    lwrb_write(&uart2_rx_rb, data, len);
}

rt_inline void _UART3_ProcessData(const void* data, size_t len)
{
    lwrb_write(&uart3_rx_rb, data, len);
}

static void _UART1_RxCheck(void)
{
    static size_t old_pos;
    size_t pos;

   /* Calculate current position in buffer and check for new data available */
    pos = LWUTIL_ARRAYSIZE(_uart1_rx_dma_buffer) - DMA_GetCurrDataCounter(UART1_DMA_RX_CHANNEL);

    /* Check change in received data */
    if (pos != old_pos)
    {
        /* Current position is over previous one */
        if (pos > old_pos)
        {
            _UART1_ProcessData(&_uart1_rx_dma_buffer[old_pos], pos - old_pos);
        }
        else
        {
            _UART1_ProcessData(&_uart1_rx_dma_buffer[old_pos], LWUTIL_ARRAYSIZE(_uart1_rx_dma_buffer) - old_pos);
            if (pos > 0)
            {
                _UART1_ProcessData(&_uart1_rx_dma_buffer[0], pos);
            }
        }
        old_pos = pos;  /* Save current position as old for next transfers */
    }
}

static void _UART2_RxCheck(void)
{
    static size_t old_pos;
    size_t pos;

    /* Calculate current position in buffer and check for new data available */
    pos = LWUTIL_ARRAYSIZE(_uart2_rx_dma_buffer) - DMA_GetCurrDataCounter(UART2_DMA_RX_CHANNEL);

    /* Check change in received data */
    if (pos != old_pos)
    {
        /* Current position is over previous one */
        if (pos > old_pos)
        {
            _UART2_ProcessData(&_uart2_rx_dma_buffer[old_pos], pos - old_pos);
        }
        else
        {
            _UART2_ProcessData(&_uart2_rx_dma_buffer[old_pos], LWUTIL_ARRAYSIZE(_uart2_rx_dma_buffer) - old_pos);
            if (pos > 0)
            {
                _UART2_ProcessData(&_uart2_rx_dma_buffer[0], pos);
            }
        }
        old_pos = pos;  /* Save current position as old for next transfers */
    }
}

static void _UART3_RxCheck(void)
{
    static size_t old_pos;
    size_t pos;

    /* Calculate current position in buffer and check for new data available */
    pos = LWUTIL_ARRAYSIZE(_uart3_rx_dma_buffer) - DMA_GetCurrDataCounter(UART3_DMA_RX_CHANNEL);

    /* Check change in received data */
    if (pos != old_pos)
    {
        /* Current position is over previous one */
        if (pos > old_pos)
        {
            _UART3_ProcessData(&_uart3_rx_dma_buffer[old_pos], pos - old_pos);
        }
        else
        {
            _UART3_ProcessData(&_uart3_rx_dma_buffer[old_pos], LWUTIL_ARRAYSIZE(_uart3_rx_dma_buffer) - old_pos);
            if (pos > 0)
            {
                _UART3_ProcessData(&_uart3_rx_dma_buffer[0], pos);
            }
        }
        old_pos = pos;  /* Save current position as old for next transfers */
    }
}

/**
 * \brief       Check if DMA is active and if not try to send data
 * \return      `1` if transfer just started, `0` if on-going or no data to transmit
 */
static uint8_t _UART1_StartTxDMATransfer(void)
{
    uint8_t started = 0;
    rt_enter_critical();

    if (_uart1_tx_dma_current_len == 0
        && (_uart1_tx_dma_current_len = lwrb_get_linear_block_read_length(&uart1_tx_rb)) > 0)
    {
        /* Limit maximal size to transmit at a time */
        // if (_uart1_tx_dma_current_len > 256)
        // {
        //     _uart1_tx_dma_current_len = 256;
        // }

        /* Disable channel if enabled */
        DMA_Cmd(UART1_DMA_TX_CHANNEL, DISABLE);

        /* Clear all flags */
        DMA_ClearFlag(DMA1_FLAG_TC4);
        DMA_ClearFlag(DMA1_FLAG_HT4);
        DMA_ClearFlag(DMA1_FLAG_GL4);
        DMA_ClearFlag(DMA1_FLAG_TE4);

        UART1_DMA_TX_CHANNEL->CNTR = _uart1_tx_dma_current_len;
        UART1_DMA_TX_CHANNEL->MADDR = (uint32_t)lwrb_get_linear_block_read_address(&uart1_tx_rb);

        /* Start transfer */
        DMA_Cmd(UART1_DMA_TX_CHANNEL, ENABLE);

        started = 1;
    }
    rt_exit_critical();

    return started;
}

/**
 * \brief       Check if DMA is active and if not try to send data
 * \return      `1` if transfer just started, `0` if on-going or no data to transmit
 */
static uint8_t _UART2_StartTxDMATransfer(void)
{
    uint8_t started = 0;
    rt_enter_critical();

    if (_uart2_tx_dma_current_len == 0
        && (_uart2_tx_dma_current_len = lwrb_get_linear_block_read_length(&uart2_tx_rb)) > 0)
    {
        /* Disable channel if enabled */
        DMA_Cmd(UART2_DMA_TX_CHANNEL, DISABLE);

        /* Limit maximal size to transmit at a time */
        if (_uart2_tx_dma_current_len > 32)
        {
            _uart2_tx_dma_current_len = 32;
        }

        /* Clear all flags */
        DMA_ClearFlag(DMA1_FLAG_TC7);
        DMA_ClearFlag(DMA1_FLAG_HT7);
        DMA_ClearFlag(DMA1_FLAG_GL7);
        DMA_ClearFlag(DMA1_FLAG_TE7);

        UART2_DMA_TX_CHANNEL->CNTR = _uart2_tx_dma_current_len;
        UART2_DMA_TX_CHANNEL->MADDR = (uint32_t)lwrb_get_linear_block_read_address(&uart2_tx_rb);

        /* Start transfer */
        DMA_Cmd(UART2_DMA_TX_CHANNEL, ENABLE);

        started = 1;
    }
    rt_exit_critical();

    return started;
}

static uint8_t _UART3_StartTxDMATransfer(void)
{
    uint8_t started = 0;

    rt_enter_critical();

    if (_uart3_tx_dma_current_len == 0
        && (_uart3_tx_dma_current_len = lwrb_get_linear_block_read_length(&uart3_tx_rb)) > 0)
    {
        /* Disable channel if enabled */
        DMA_Cmd(UART3_DMA_TX_CHANNEL, DISABLE);

        /* Clear all flags */
        DMA_ClearFlag(DMA1_FLAG_TC2);
        DMA_ClearFlag(DMA1_FLAG_HT2);
        DMA_ClearFlag(DMA1_FLAG_GL2);
        DMA_ClearFlag(DMA1_FLAG_TE2);

        UART3_DMA_TX_CHANNEL->CNTR = _uart3_tx_dma_current_len;
        UART3_DMA_TX_CHANNEL->MADDR = (uint32_t)lwrb_get_linear_block_read_address(&uart3_tx_rb);

        /* Start transfer */
        DMA_Cmd(UART3_DMA_TX_CHANNEL, ENABLE);

        started = 1;
    }

    rt_exit_critical();

    return started;
}

rt_uint32_t UART1_Write(const void* data, size_t len)
{
    rt_uint32_t ret = 0;
    if (lwrb_get_free(&uart1_tx_rb) >= len)
    {
        ret = lwrb_write(&uart1_tx_rb, data, len);
        _UART1_StartTxDMATransfer(); /* Then try to start transfer */
    }
    return ret;
}

/**
 * \brief           Send string to UART1
 * \param[in]       str: String to send
 */
rt_uint32_t UART1_SendString(const char* str)
{
    rt_uint32_t ret = 0;
    rt_uint32_t len = rt_strlen(str);
    UART1_Write(str, len);
    return ret;
}

rt_uint32_t UART2_Write(const void* data, size_t len)
{
    rt_uint32_t ret = 0;
    if (lwrb_get_free(&uart2_tx_rb) >= len)
    {
        ret = lwrb_write(&uart2_tx_rb, data, len);
        _UART2_StartTxDMATransfer(); /* Then try to start transfer */
    }

    return ret;
}

/**
 * \brief           Send string to UART2
 * \param[in]       str: String to send
 */
rt_uint32_t UART2_SendString(const char* str)
{
    rt_uint32_t ret = 0;
    rt_uint32_t len = rt_strlen(str);
    UART2_Write(str, len);
    return ret;
}

rt_uint32_t UART3_Write(const void* data, size_t len)
{
    rt_uint32_t ret = 0;
    if (lwrb_get_free(&uart3_tx_rb) >= len)
    {
        ret = lwrb_write(&uart3_tx_rb, data, len);
        _UART3_StartTxDMATransfer(); /* Then try to start transfer */
    }
    return ret;
}

/**
 * \brief           Send string to UART3
 * \param[in]       str: String to send
 */
rt_uint32_t UART3_SendString(const char* str)
{
    rt_uint32_t ret = 0;
    rt_uint32_t len = rt_strlen(str);
    UART3_Write(str, len);
    return ret;
}

rt_uint32_t UART3_Read(void *buf, rt_uint32_t len)
{
    return lwrb_read(&uart3_rx_rb, buf, len);
}

static void uart1_rx_dma_thread_entry(void* parameter)
{
    LOG_D("uart1_rx_dma_thread_entry");

    while (1)
    {
        rt_sem_take(uart1_rx_check_sem, RT_WAITING_FOREVER);
        _UART1_RxCheck();
    }
}

static void uart2_rx_dma_thread_entry(void* parameter)
{
    LOG_D("uart2_rx_dma_thread_entry");

    while (1)
    {
        rt_sem_take(uart2_rx_check_sem, RT_WAITING_FOREVER);
        _UART2_RxCheck();
    }
}

static void uart3_rx_dma_thread_entry(void* parameter)
{
    LOG_D("uart3_rx_dma_thread_entry");

    while (1)
    {
        rt_sem_take(uart3_rx_check_sem, RT_WAITING_FOREVER);
        _UART3_RxCheck();
    }
}

// TODO: 用预编译BSP_USING_UART1
void UART1_Init(uint32_t baudrate, TeUsartParityMode parity)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};    // DMA 配置

    /* Initialize ringbuff */
    lwrb_init(&uart1_rx_rb, uart1_rx_rb_data, sizeof(uart1_rx_rb_data));
    lwrb_init(&uart1_tx_rb, uart1_tx_rb_data, sizeof(uart1_tx_rb_data));

    USART_DeInit(USART1);   // 寄存器恢复默认值

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* 串口1 GPIO引脚初始化 */
    /* USART1 TX-->A.9   RX-->A.10 */
    GPIO_InitStructure.GPIO_Pin = UART1_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART1_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UART1_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART1_RX_GPIO_PORT, &GPIO_InitStructure);

    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_PE, ENABLE);

    /* 串口DMA配置 */
#if 1
    /* UART1 RX DMA init */
    DMA_DeInit(UART1_DMA_RX_CHANNEL);  // DMA1 通道5,寄存器复位, uart1_rx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)_uart1_rx_dma_buffer; // 接收内存地址，从哪里接收
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(_uart1_rx_dma_buffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(UART1_DMA_RX_CHANNEL, &DMA_InitStructure);
#endif
    // /* UART1 TX DMA init */
    DMA_DeInit(UART1_DMA_TX_CHANNEL);  // DMA1 通道4,寄存器复位, uart1_tx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart1_tx_rb_data;  //传输再修改地址，并使能该通道
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(uart1_tx_rb_data);
    // DMA_InitStructure.DMA_BufferSize = (uint32_t)0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(UART1_DMA_TX_CHANNEL, &DMA_InitStructure);

    /* Enable HT & TC interrupts for RX */
    DMA_ITConfig(UART1_DMA_RX_CHANNEL, DMA_IT_HT, ENABLE);
    DMA_ITConfig(UART1_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);

    /* Enable TC interrupts for TX */
    DMA_ITConfig(UART1_DMA_TX_CHANNEL, DMA_IT_TC, ENABLE);

    /* 应用程序必须确保 DMA 和 UART 中断使用相同的抢占优先级。
        这是保证处理功能永远不会被自身抢占的唯一配置
        （DMA 中断抢占 UART，或相反），否则最后已知的读取位置可能会损坏
        ，并且应用程序将使用错误的数据进行操作。*/
    /* DMA interrupt init for RX & TX */
    NVIC_InitStructure.NVIC_IRQChannel = UART1_DMA_TX_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART1_DMA_RX_IRQ_CHANNEL;
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
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable UART and DMA */
    DMA_Cmd(UART1_DMA_RX_CHANNEL, ENABLE);
    DMA_Cmd(UART1_DMA_TX_CHANNEL, DISABLE);

    USART_Cmd(USART1, ENABLE);
    LOG_I("USART1 Init");
}

static int _UART1_SemCreate(void)
{
    uart1_rx_parity_err_sem = rt_sem_create("uart1_rx_parity_err_sem", 0, RT_IPC_FLAG_PRIO);
    if (uart1_rx_parity_err_sem != RT_NULL)
    {
        LOG_D("create uart1_rev_parity_err_sem");
    }

    uart1_rx_check_sem = rt_sem_create("uart1_rx_check_sem", 0, RT_IPC_FLAG_PRIO);
    if (uart1_rx_check_sem != RT_NULL)
    {
        LOG_D("create uart1_rx_check_sem create");
    }

    uart1_rx_ok_sem = rt_sem_create("uart1_rx_ok_sem", 0, RT_IPC_FLAG_PRIO);
    if (uart1_rx_ok_sem != RT_NULL)
    {
        LOG_D("create uart1_rx_ok_sem create");
    }

    if (rt_thread_init(&uart1_dma_rx_thread, "uart1_dma_rx_thread"
                            , uart1_rx_dma_thread_entry
                            , RT_NULL
                            , &uart1_dma_rx_thread_stack[0]
                            , UART1_DMA_RX_THREAD_STACK_SIZE
                            , UART1_DMA_RX_THREAD_PRIORITY
                            , UART1_DMA_RX_THREAD_TIMESLICE)
            == RT_EOK)
    {
        LOG_D("startup uart1_dma_rx_thread = %d"
            , rt_thread_startup(&uart1_dma_rx_thread));
    }

    return 0;
}
INIT_PREV_EXPORT(_UART1_SemCreate);

void UART2_Init(uint32_t baudrate, TeUsartParityMode parity)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};			// DMA 配置

    /* Initialize ringbuff for TX & RX */
    lwrb_init(&uart2_rx_rb, uart2_rx_rb_data, sizeof(uart2_rx_rb_data));
    lwrb_init(&uart2_tx_rb, uart2_tx_rb_data, sizeof(uart2_tx_rb_data));
    // lwrb_set_evt_fn(&uart2_rx_fifo, USART2_BufferEvent);

    USART_DeInit(USART2);   // 寄存器恢复默认值

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* 串口2 GPIO引脚初始化 */
    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = UART2_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART2_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UART2_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART2_RX_GPIO_PORT, &GPIO_InitStructure);

    // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_PE, ENABLE);

    /* 串口DMA配置 */
    /* UART2 RX DMA init */
    DMA_DeInit(UART2_DMA_RX_CHANNEL);  // DMA1 通道6,寄存器复位, uart2_rx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DATAR); /* USART2->DATAR:0x40004404 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)_uart2_rx_dma_buffer; // 接收内存地址，从哪里接收
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(_uart2_rx_dma_buffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(UART2_DMA_RX_CHANNEL, &DMA_InitStructure);

    // /* UART2 TX DMA init */
    DMA_DeInit(UART2_DMA_TX_CHANNEL);  // DMA1 通道7,寄存器复位, uart2_tx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DATAR); /* USART2->DATAR:0x40004404 */
    // DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart2_tx_rb_data;  //传输再修改地址，并使能该通道
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    // DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(uart2_tx_rb_data);
    // DMA_InitStructure.DMA_BufferSize = (uint32_t)0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(UART2_DMA_TX_CHANNEL, &DMA_InitStructure);

    /* Enable HT & TC interrupts for RX */
    DMA_ITConfig(UART2_DMA_RX_CHANNEL, DMA_IT_HT, ENABLE);
    DMA_ITConfig(UART2_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);

    /* Enable TC interrupts for TX */
    DMA_ITConfig(UART2_DMA_TX_CHANNEL, DMA_IT_TC, ENABLE);

    /* DMA interrupt init for RX & TX */
    NVIC_InitStructure.NVIC_IRQChannel = UART2_DMA_RX_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART2_DMA_TX_IRQ_CHANNEL;
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
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable UART and DMA */
    DMA_Cmd(UART2_DMA_RX_CHANNEL, ENABLE);
    DMA_Cmd(UART2_DMA_TX_CHANNEL, DISABLE);

    USART_Cmd(USART2, ENABLE);

    LOG_I("USART2 Init");
}

static int _UART2_SemCreate(void)
{
    uart2_rx_parity_err_sem = rt_sem_create("uart2_rx_parity_err_sem", 0, RT_IPC_FLAG_PRIO);
    if (uart2_rx_parity_err_sem != RT_NULL)
    {
        LOG_D("create uart2_rx_parity_err_sem");
    }
    uart2_rx_check_sem = rt_sem_create("uart2_rx_check_sem", 0, RT_IPC_FLAG_PRIO);
    if (uart2_rx_check_sem != RT_NULL)
    {
        LOG_D("create uart2_rx_check_sem");
    }
    uart2_rx_ok_sem = rt_sem_create("uart2_rx_ok_sem", 0, RT_IPC_FLAG_PRIO);
    if (uart2_rx_ok_sem != RT_NULL)
    {
        LOG_D("create uart2_rx_ok_sem create");
    }

    if (rt_thread_init(&uart2_dma_rx_thread, "uart2_dma_rx_thread"
                            , uart2_rx_dma_thread_entry
                            , RT_NULL
                            , &uart2_dma_rx_thread_stack[0]
                            , UART2_DMA_RX_THREAD_STACK_SIZE
                            , UART2_DMA_RX_THREAD_PRIORITY
                            , UART2_DMA_RX_THREAD_TIMESLICE)
            == RT_EOK)
    {
        LOG_D("startup uart2_dma_rx_thread = %d"
            , rt_thread_startup(&uart2_dma_rx_thread));
    }

    return 0;
}
INIT_PREV_EXPORT(_UART2_SemCreate);

void UART3_Init(uint32_t baudrate, TeUsartParityMode parity)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};			// DMA 配置

    /* Initialize ringbuff for TX & RX */
    lwrb_init(&uart3_rx_rb, uart3_rx_rb_data, sizeof(uart3_rx_rb_data));
    lwrb_init(&uart3_tx_rb, uart3_tx_rb_data, sizeof(uart3_tx_rb_data));
    // lwrb_set_evt_fn(&uart2_rx_fifo, USART2_BufferEvent);

    USART_DeInit(USART3);   // 寄存器恢复默认值

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);

    /* 串口3 GPIO引脚初始化 */
    /* USART3 TX-->B.10   RX-->B.11 */
    GPIO_InitStructure.GPIO_Pin = UART3_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART3_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UART3_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART3_RX_GPIO_PORT, &GPIO_InitStructure);

    // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_PE, ENABLE);

    /* 串口DMA配置 */
    /* UART2 RX DMA init */
    DMA_DeInit(UART3_DMA_RX_CHANNEL);  // DMA1 通道6,寄存器复位, uart2_rx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DATAR); /* USART2->DATAR:0x40004404 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)_uart3_rx_dma_buffer; // 接收内存地址，从哪里接收
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(_uart3_rx_dma_buffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(UART3_DMA_RX_CHANNEL, &DMA_InitStructure);

    // /* UART2 TX DMA init */
    DMA_DeInit(UART3_DMA_TX_CHANNEL);  // DMA1 通道7,寄存器复位, uart3_tx
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DATAR); /* USART3->DATAR:0x40004404 */
    // DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart3_tx_rb_data;  //传输再修改地址，并使能该通道
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    // DMA_InitStructure.DMA_BufferSize = LWUTIL_ARRAYSIZE(uart3_tx_rb_data);
    // DMA_InitStructure.DMA_BufferSize = (uint32_t)0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(UART3_DMA_TX_CHANNEL, &DMA_InitStructure);

    /* Enable HT & TC interrupts for RX */
    DMA_ITConfig(UART3_DMA_RX_CHANNEL, DMA_IT_HT, ENABLE);
    DMA_ITConfig(UART3_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);

    /* Enable TC interrupts for TX */
    DMA_ITConfig(UART3_DMA_TX_CHANNEL, DMA_IT_TC, ENABLE);

    /* DMA interrupt init for RX & TX */
    NVIC_InitStructure.NVIC_IRQChannel = UART3_DMA_RX_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART3_DMA_TX_IRQ_CHANNEL;
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
    DMA_Cmd(UART3_DMA_RX_CHANNEL, ENABLE);
    DMA_Cmd(UART3_DMA_TX_CHANNEL, DISABLE);

    USART_Cmd(USART3, ENABLE);

    LOG_I("USART3 Init");
}

static int _UART3_SemCreate(void)
{
    uart3_rx_parity_err_sem = rt_sem_create("uart3_rx_parity_err_sem", 0, RT_IPC_FLAG_PRIO);
    if (uart3_rx_parity_err_sem != RT_NULL)
    {
        LOG_D("create uart3_rx_parity_err_sem");
    }
    uart3_rx_check_sem = rt_sem_create("uart3_rx_check_sem", 0, RT_IPC_FLAG_PRIO);
    if (uart3_rx_check_sem != RT_NULL)
    {
        LOG_D("create uart3_rx_check_sem create");
    }
    uart3_rx_ok_sem = rt_sem_create("uart3_rx_ok_sem", 0, RT_IPC_FLAG_PRIO);
    if (uart3_rx_ok_sem != RT_NULL)
    {
        LOG_D("create uart3_rx_ok_sem create");
    }

    if (rt_thread_init(&uart3_dma_rx_thread, "uart3_dma_rx_thread"
                            , uart3_rx_dma_thread_entry
                            , RT_NULL
                            , &uart3_dma_rx_thread_stack[0]
                            , UART3_DMA_RX_THREAD_STACK_SIZE
                            , UART3_DMA_RX_THREAD_PRIORITY
                            , UART3_DMA_RX_THREAD_TIMESLICE)
            == RT_EOK)
    {
        LOG_D("startup uart3_dma_rx_thread = %d"
            , rt_thread_startup(&uart3_dma_rx_thread));
    }

    return 0;
}
INIT_PREV_EXPORT(_UART3_SemCreate);

// DMA1 channel2 interrupt handler for UART3 TX
void DMA1_Channel2_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC2) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC2); /* Clear transfer complete flag */
        lwrb_skip(&uart3_tx_rb, _uart3_tx_dma_current_len);/* Skip buffer, it has been successfully sent out */
        _uart3_tx_dma_current_len = 0;      /* Reset data length */
        _UART3_StartTxDMATransfer();        /* Start new transfer */
    }

    /* Implement other events when needed */

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

// UART3 RX
void DMA1_Channel3_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    /* Check half-transfer complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_HT3) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_HT3); /* Clear half-transfer complete flag */
        // UART3_RxCheck();                       /* Check data */
        // LOG_D("DMA1_IT_HT3");
        rt_sem_release(uart3_rx_check_sem);
    }
    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC3) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC3); /* Clear transfer complete flag */
        // UART3_RxCheck();                       /* Check data */
        // LOG_D("DMA1_IT_TC3");
        rt_sem_release(uart3_rx_check_sem);
    }

    /* Implement other events when needed */

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

// UART1 TX
void DMA1_Channel4_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC4) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC4); /* Clear transfer complete flag */
        lwrb_skip(&uart1_tx_rb, _uart1_tx_dma_current_len);/* Skip buffer, it has been successfully sent out */
        _uart1_tx_dma_current_len = 0;           /* Reset data length */
        _UART1_StartTxDMATransfer();          /* Start new transfer */
    }

    /* Implement other events when needed */

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

//  UART1 RX
void DMA1_Channel5_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    /* Check half-transfer complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_HT5) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_HT5); /* Clear half-transfer complete flag */
        // UART1_RxCheck();                       /* Check data */
        rt_sem_release(uart1_rx_check_sem);
        // LOG_D("DMA1_IT_HT5");
    }
    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC5) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC5); /* Clear transfer complete flag */
        // UART1_RxCheck();                       /* Check data */
        rt_sem_release(uart1_rx_check_sem);
        // LOG_D("DMA1_IT_TC5");
    }
    /* Implement other events when needed */

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

// UART2 RX
void DMA1_Channel6_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    /* Check half-transfer complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_HT6) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_HT6); /* Clear half-transfer complete flag */
        // UART2_RxCheck();                       /* Check data */
        // LOG_D("DMA1_IT_HT6");
        rt_sem_release(uart2_rx_check_sem);
    }
    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC6) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC6); /* Clear transfer complete flag */
        // UART2_RxCheck();                       /* Check data */
        // LOG_D("DMA1_IT_TC6");
        rt_sem_release(uart2_rx_check_sem);
    }

    /* Implement other events when needed */

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

// UART2 TX
void DMA1_Channel7_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    /* Check transfer-complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC7) != DISABLE)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC7); /* Clear transfer complete flag */
        lwrb_skip(&uart2_tx_rb, _uart2_tx_dma_current_len);/* Skip buffer, it has been successfully sent out */
        _uart2_tx_dma_current_len = 0;           /* Reset data length */
        _UART2_StartTxDMATransfer();          /* Start new transfer */
    }

    /* Implement other events when needed */

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

void USART1_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    if(USART_GetITStatus(USART1, USART_IT_PE) != RESET) //校验错误
    {
        rt_sem_release(uart1_rx_parity_err_sem);
        // rt_event_send(&uart1_event, UART_EVENT_IT_RX_PE_FLAG);
    }
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//接收完数据后进入空闲中断
    {
        uint8_t temp;
        // 如果IDLEIE已经被置位，则会产生对应的中断。读状态寄存器再读数据寄存器的操作会清除此位
        temp = USART1->STATR;
        temp = USART1->DATAR;
        temp &= 0;

        rt_sem_release(uart1_rx_check_sem);
        rt_sem_release(uart1_rx_ok_sem);
        // rt_event_send(&uart1_event, UART_EVENT_IT_RX_IDLE_FLAG);
    }
    /* Implement other events when needed */

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

/**
 * @description: UART2 global interrupt handler
 * @return {*}
 */
void USART2_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    if(USART_GetITStatus(USART2, USART_IT_PE) != RESET)//校验错误
    {
        // uart2_rev_parity_flag = 1;
        // 这里有校验错误的话，信号量uart2_rx_parity_err_sem会加好几次
        // LOG_D("uart2_rx_parity_err_sem");
        rt_sem_release(uart2_rx_parity_err_sem);
    }
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)//接收完数据后进入空闲中断
    {
        uint8_t temp;
        // 如果IDLEIE已经被置位，则会产生对应的中断。读状态寄存器再读数据寄存器的操作会清除此位
  	    temp = USART2->STATR;
        temp = USART2->DATAR;
        temp &= 0;

        // UART2_RxCheck();
        // uart2_rev_flag = 1; //检测到空闲状态，置位接收完成位
        // LOG_D("USART2_IRQHandler");
        rt_sem_release(uart2_rx_check_sem);
        rt_sem_release(uart2_rx_ok_sem);
    }
    /* Implement other events when needed */

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

void USART3_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    if(USART_GetITStatus(USART3, USART_IT_PE) != RESET)//校验错误
    {
        // uart3_rev_parity_flag = 1; //校验错误
        rt_sem_release(uart3_rx_parity_err_sem);
    }
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)//接收完数据后进入空闲中断
    {
        uint8_t temp;
        // 如果IDLEIE已经被置位，则会产生对应的中断。读状态寄存器再读数据寄存器的操作会清除此位
  	    temp = USART3->STATR;
        temp = USART3->DATAR;
        temp &= 0;

        // UART3_RxCheck();
        rt_sem_release(uart3_rx_check_sem);
        rt_sem_release(uart3_rx_ok_sem);
    }
    /* Implement other events when needed */

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

int rt_hw_usart_init(void)
{
    UART1_Init(115200, kCheck0Stop1);
    // UART2_Init(115200, kCheck0Stop1);
    // UART3_Init(115200, kCheck0Stop1);

    return 0;
}

/* 移植控制台，实现控制台输出, 对接 rt_hw_console_output */
void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0, j = 0;
    char a = '\r';

    size = rt_strlen(str);
    rt_uint8_t buf[size + size / 10 + 10];

    for (i = 0; i < size; i++, j++)
    {
        if (*(str + i) == '\n')
        {
            // UART1_Write(&a, 1);
            buf[j] = a;
            j++;
        }
        // UART1_Write((str + i), 1);
        buf[j] = str[i];
    }

    UART1_Write(buf, j);
}

/* 移植 FinSH，实现命令行交互, 需要添加 FinSH 源码，然后再对接 rt_hw_console_getchar */
char rt_hw_console_getchar(void)
{
    char ch = 0;

    if (lwrb_get_full(&uart1_rx_rb) > 0)
    {
        lwrb_read(&uart1_rx_rb, &ch, 1);
    }
    else
    {
        rt_sem_take(uart1_rx_ok_sem, RT_WAITING_FOREVER);
    }

    return ch;
}


#endif /* RT_USING_SERIAL */

