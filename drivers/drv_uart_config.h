/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     SummerGift   first version
 * 2019-01-03     zylx         modify dma support
 * 2020-06-03     chenyaxing   modify uart config
 */

#ifndef __DRV_UART_CONFIG_H__
#define __DRV_UART_CONFIG_H__

#include <rtthread.h>
#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __DMA_CHANNEL(x, y)     DMA##x##_Channel##y
#define __DMA_IRQ(x, y)         DMA##x##_Channel##y##_IRQHandler

#define __DMA_FLAG_GL(x, y)     DMA##x##_FLAG_GL_##y
#define __DMA_FLAG_TC(x, y)     DMA##x##_FLAG_TC_##y
#define __DMA_FLAG_HT(x, y)     DMA##x##_FLAG_HT_##y
#define __DMA_FLAG_TE(x, y)     DMA##x##_FLAG_TE_##y

#define __DMA_IT_GL(x, y)     DMA##x##_IT_GL_##y
#define __DMA_IT_TC(x, y)     DMA##x##_IT_TC_##y
#define __DMA_IT_HT(x, y)     DMA##x##_IT_HT_##y
#define __DMA_IT_TE(x, y)     DMA##x##_IT_TE_##y

#if defined(BSP_USING_UART1)
#ifndef UART1_CONFIG

#define __UART1_DMA               1
#define __UART1_TX_DMA_CHANNEL    4
#define __UART1_RX_DMA_CHANNEL    5

#define UART1_CONFIG                        \
    {                                       \
        .UartInstance = USART1,             \
        .uart_irq = USART1_IRQHandler,      \

        .uart_tx_port = GPIOA,              \
        .uart_rx_port = GPIOA,              \
        .uart_tx_pin = GPIO_PIN_9,          \
        .uart_rx_pin = GPIO_PIN_10,         \
        .gpio_remap = 0,                    \
        .gpio_remap_state = DISABLE,        \

        .dma_tx.DmaChannel = __DMA_CHANNEL(__UART1_DMA, __UART1_TX_DMA_CHANNEL), \
        .dma_rx.DmaChannel = __DMA_CHANNEL(__UART1_DMA, __UART1_RX_DMA_CHANNEL), \

        .dma_tx.dma_irq = __DMA_IRQ(__UART1_DMA, __UART1_RX_DMA_CHANNEL), \
        .dma_rx.dma_irq = __DMA_IRQ(__UART1_DMA, __UART1_RX_DMA_CHANNEL), \

        .dma_tx.dma_flag_gl = __DMA_FLAG_GL(__UART1_DMA, __UART1_TX_DMA_CHANNEL),  \
        .dma_tx.dma_flag_tc = __DMA_FLAG_TC(__UART1_DMA, __UART1_TX_DMA_CHANNEL),  \
        .dma_tx.dma_flag_ht = __DMA_FLAG_HT(__UART1_DMA, __UART1_TX_DMA_CHANNEL),  \
        .dma_tx.dma_flag_te = __DMA_FLAG_TE(__UART1_DMA, __UART1_TX_DMA_CHANNEL),  \

        .dma_tx.dma_it_gl = __DMA_IT_GL(__UART1_DMA, __UART1_TX_DMA_CHANNEL),  \
        .dma_tx.dma_it_tc = __DMA_IT_TC(__UART1_DMA, __UART1_TX_DMA_CHANNEL),  \
        .dma_tx.dma_it_ht = __DMA_IT_HT(__UART1_DMA, __UART1_TX_DMA_CHANNEL),  \
        .dma_tx.dma_it_te = __DMA_IT_TE(__UART1_DMA, __UART1_TX_DMA_CHANNEL),  \

        .dma_rx.dma_flag_gl = __DMA_FLAG_GL(__UART1_DMA, __UART1_RX_DMA_CHANNEL),  \
        .dma_rx.dma_flag_tc = __DMA_FLAG_TC(__UART1_DMA, __UART1_RX_DMA_CHANNEL),  \
        .dma_rx.dma_flag_ht = __DMA_FLAG_HT(__UART1_DMA, __UART1_RX_DMA_CHANNEL),  \
        .dma_rx.dma_flag_te = __DMA_FLAG_TE(__UART1_DMA, __UART1_RX_DMA_CHANNEL),  \

        .dma_rx.dma_it_gl = __DMA_IT_GL(__UART1_DMA, __UART1_RX_DMA_CHANNEL),  \
        .dma_rx.dma_it_tc = __DMA_IT_TC(__UART1_DMA, __UART1_RX_DMA_CHANNEL),  \
        .dma_rx.dma_it_ht = __DMA_IT_HT(__UART1_DMA, __UART1_RX_DMA_CHANNEL),  \
        .dma_rx.dma_it_te = __DMA_IT_TE(__UART1_DMA, __UART1_RX_DMA_CHANNEL),  \
    }
#endif /* UART1_CONFIG */
#endif /* BSP_USING_UART1 */

#ifdef __cplusplus
}
#endif

#endif  // !__DRV_UART_CONFIG_H__
