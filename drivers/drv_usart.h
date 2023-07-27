/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     WCH        the first version
 */
#ifndef __DRV_USART_H__
#define __DRV_USART_H__

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

typedef enum
{
    kCheck0Stop1 = 0,
    kCheck0Stop2,
    kCheckOdd,
    kCheckEven
} TeUsartParityMode;

// UART1 - TX
#define USART1_TX_GPIO_PIN              GPIO_Pin_9
#define USART1_TX_GPIO_PORT             GPIOA
// UART1 - RX
#define USART1_RX_GPIO_PIN              GPIO_Pin_10
#define USART1_RX_GPIO_PORT             GPIOA

#define USART1_RX_BUFFER_LENGTH         (128U)
#define USART1_TX_RB_LENGTH             (2*1024U)

#define USART1_DMA_RX_CHANNEL           DMA1_Channel5
#define USART1_DMA_TX_CHANNEL           DMA1_Channel4

#define USART1_DMA_RX_IRQ_CHANNEL       DMA1_Channel5_IRQn
#define USART1_DMA_TX_IRQ_CHANNEL       DMA1_Channel4_IRQn

// UART2 - TX
#define USART2_TX_GPIO_PIN              GPIO_Pin_2
#define USART2_TX_GPIO_PORT             GPIOA
// UART2 - RX
#define USART2_RX_GPIO_PIN              GPIO_Pin_3
#define USART2_RX_GPIO_PORT             GPIOA

#define USART2_RX_BUFFER_LENGTH         (128U)
#define USART2_TX_RB_LENGTH             (128U)

#define USART2_DMA_RX_CHANNEL           DMA1_Channel6
#define USART2_DMA_TX_CHANNEL           DMA1_Channel7

#define USART2_DMA_RX_IRQ_CHANNEL       DMA1_Channel6_IRQn
#define USART2_DMA_TX_IRQ_CHANNEL       DMA1_Channel7_IRQn

// UART3 - TX
#define USART3_TX_GPIO_PIN              GPIO_Pin_10
#define USART3_TX_GPIO_PORT             GPIOB
// UART3 - RX
#define USART3_RX_GPIO_PIN              GPIO_Pin_11
#define USART3_RX_GPIO_PORT             GPIOB

#define USART3_RX_BUFFER_LENGTH         (512U)
#define USART3_TX_RB_LENGTH             (512U)

#define USART3_DMA_RX_CHANNEL           DMA1_Channel3
#define USART3_DMA_TX_CHANNEL           DMA1_Channel2

#define USART3_DMA_RX_IRQ_CHANNEL       DMA1_Channel3_IRQn
#define USART3_DMA_TX_IRQ_CHANNEL       DMA1_Channel2_IRQn

extern lwrb_t usart1_rx_rb;
extern volatile uint8_t uart1_rev_flag; //  串口2接收到数据待解析的

extern lwrb_t usart2_rx_rb;
extern volatile uint8_t uart2_rev_flag; //  串口2接收到数据待解析的

extern lwrb_t usart3_rx_rb;
extern volatile uint8_t uart3_rev_flag; //  串口2接收到数据待解析的

// 阻塞式发送的
void USART_SendByte(USART_TypeDef *pUSARTx, const uint16_t data);
void USART_SendString(USART_TypeDef *pUSARTx, const char *str);
void USART_SendArray(USART_TypeDef *pUSARTx, const uint8_t *pdata, uint16_t len);

void USART1_Init(uint32_t baudrate, TeUsartParityMode parity);
void USART1_SendString(const char* str);
void USART1_SendArray(const void* data, size_t len);

void USART2_Init(uint32_t baudrate, TeUsartParityMode parity);
void USART2_SendString(const char* str);
void USART2_SendArray(const void* data, size_t len);

void USART3_Init(uint32_t baudrate, TeUsartParityMode parity);
void USART3_SendString(const char* str);
unsigned int USART3_SendArray(const void* data, unsigned int len);
unsigned int UART3_Read(void *buf, unsigned int len);

#endif // !RT_USING_SERIAL


int rt_hw_usart_init(void);


#endif
