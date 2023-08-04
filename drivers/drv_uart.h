/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     WCH        the first version
 */
#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include "board.h"
#include <rtthread.h>
#include "rtdevice.h"
#include <rthw.h>

#ifdef RT_USING_SERIAL
#include "serial.h"
/* ch32 config class */
struct ch32_uart_config
{
    const char *name;
    USART_TypeDef *Instance;
    IRQn_Type irq_type;
};

/* ch32 uart dirver class */
struct ch32_uart
{
    struct ch32_uart_config *config;
    USART_InitTypeDef Init;
    struct rt_serial_device serial;
};
#else
#include "lwrb.h"

// 阻塞式发送的
// #define UART_USE_POLLING_TX

#define UART1_DMA_RX_THREAD_STACK_SIZE    (512)
#define UART1_DMA_RX_THREAD_PRIORITY      (20)
#define UART1_DMA_RX_THREAD_TIMESLICE     (5)

#define UART2_DMA_RX_THREAD_STACK_SIZE    (512)
#define UART2_DMA_RX_THREAD_PRIORITY      (9)
#define UART2_DMA_RX_THREAD_TIMESLICE     (5)

#define UART3_DMA_RX_THREAD_STACK_SIZE    (512)
#define UART3_DMA_RX_THREAD_PRIORITY      (8)
#define UART3_DMA_RX_THREAD_TIMESLICE     (5)

// extern rt_event_t uart2_event;

// // 收到一帧完成数据
// #define UART2_EVENT_RX_OK_FLAG      (1 << 0)
// // 接收数据中有校验错误发生
// #define UART2_EVENT_RX_PE_FLAG      (1 << 1)

// UART1 - TX
#define UART1_TX_GPIO_PIN              GPIO_Pin_9
#define UART1_TX_GPIO_PORT             GPIOA
// UART1 - RX
#define UART1_RX_GPIO_PIN              GPIO_Pin_10
#define UART1_RX_GPIO_PORT             GPIOA

#define UART1_RX_BUFFER_LENGTH         (64U)
#define UART1_RX_RB_LENGTH             (72U)
#define UART1_TX_RB_LENGTH             (2*1024U)

#define UART1_DMA_RX_CHANNEL           DMA1_Channel5
#define UART1_DMA_TX_CHANNEL           DMA1_Channel4

#define UART1_DMA_RX_IRQ_CHANNEL       DMA1_Channel5_IRQn
#define UART1_DMA_TX_IRQ_CHANNEL       DMA1_Channel4_IRQn

// UART2 - TX
#define UART2_TX_GPIO_PIN              GPIO_Pin_2
#define UART2_TX_GPIO_PORT             GPIOA
// UART2 - RX
#define UART2_RX_GPIO_PIN              GPIO_Pin_3
#define UART2_RX_GPIO_PORT             GPIOA

#define UART2_RX_BUFFER_LENGTH         (64U)
#define UART2_RX_RB_LENGTH             (72U)
#define UART2_TX_RB_LENGTH             (64U)

#define UART2_DMA_RX_CHANNEL           DMA1_Channel6
#define UART2_DMA_TX_CHANNEL           DMA1_Channel7

#define UART2_DMA_RX_IRQ_CHANNEL       DMA1_Channel6_IRQn
#define UART2_DMA_TX_IRQ_CHANNEL       DMA1_Channel7_IRQn

// UART3 - TX
#define UART3_TX_GPIO_PIN              GPIO_Pin_10
#define UART3_TX_GPIO_PORT             GPIOB
// UART3 - RX
#define UART3_RX_GPIO_PIN              GPIO_Pin_11
#define UART3_RX_GPIO_PORT             GPIOB

#define UART3_RX_BUFFER_LENGTH         (512U)
#define UART3_RX_RB_LENGTH             (520U)
#define UART3_TX_RB_LENGTH             (512U)

#define UART3_DMA_RX_CHANNEL           DMA1_Channel3
#define UART3_DMA_TX_CHANNEL           DMA1_Channel2

#define UART3_DMA_RX_IRQ_CHANNEL       DMA1_Channel3_IRQn
#define UART3_DMA_TX_IRQ_CHANNEL       DMA1_Channel2_IRQn

// extern lwrb_t uart1_rx_rb;
extern lwrb_t uart2_rx_rb;
extern lwrb_t uart3_rx_rb;

// uart2 历史数据读取 4800bps偶校验
extern rt_sem_t uart2_rx_ok_sem;
extern rt_sem_t uart2_rx_parity_err_sem;

extern rt_sem_t uart3_rx_ok_sem;

// #define UART_EVENT_DMA_RX_GL_FLAG       (1 << 0)
// #define UART_EVENT_DMA_RX_TC_FLAG       (1 << 1)
// #define UART_EVENT_DMA_RX_HT_FLAG       (1 << 2)
// #define UART_EVENT_DMA_RX_TE_FLAG       (1 << 3)

// #define UART_EVENT_DMA_TX_GL_FLAG       (1 << 4)
// #define UART_EVENT_DMA_TX_TC_FLAG       (1 << 5)
// #define UART_EVENT_DMA_TX_HT_FLAG       (1 << 6)
// #define UART_EVENT_DMA_TX_TE_FLAG       (1 << 7)

// #define UART_EVENT_IT_RX_IDLE_FLAG      (1 << 8)
// #define UART_EVENT_IT_RX_PE_FLAG        (1 << 9)

// // 一帧数据接受完成
// #define UART_EVENT_RX_OK_FLAG           (1 << 10)
// #define UART_EVENT_TX_OK_FLAG           (1 << 11)

typedef enum
{
    kCheck0Stop1 = 0U,
    kCheck0Stop2,
    kCheckOdd,
    kCheckEven
} TeUsartParityMode;

#ifdef UART_USE_POLLING_TX
void UART_SendByte(USART_TypeDef *pUSARTx, const uint16_t data);
void UART_SendString(USART_TypeDef *pUSARTx, const char *str);
void UART_SendArray(USART_TypeDef *pUSARTx, const uint8_t *pdata, uint16_t len);
#endif // !UART_USE_POLLING_TX

void UART1_Init(uint32_t baudrate, TeUsartParityMode parity);
rt_uint32_t UART1_Write(const void* data, size_t len);
rt_uint32_t UART1_SendString(const char* str);

void UART2_Init(uint32_t baudrate, TeUsartParityMode parity);
rt_uint32_t UART2_Write(const void* data, size_t len);
rt_uint32_t UART2_SendString(const char* str);

void UART3_Init(uint32_t baudrate, TeUsartParityMode parity);
rt_uint32_t UART3_Write(const void* data, size_t len);
rt_uint32_t UART3_SendString(const char* str);
rt_uint32_t UART3_Read(void *buf, rt_uint32_t len);

#endif // !RT_USING_SERIAL


int rt_hw_usart_init(void);


#endif // !__DRV_UART_H__
