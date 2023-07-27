/*
 * @Author       : yzy
 * @Date         : 2023-05-31 14:07:24
 * @LastEditors  : yzy
 * @LastEditTime : 2023-06-15 17:22:03
 * @FilePath     : \DT563_CH32V203C8T6_Cat1\common\letter-shell\shell_port.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
/*
 * @Author       : yzy
 * @Date         : 2022-11-24 13:24:59
 * @LastEditors  : yzy
 * @LastEditTime : 2023-02-01 13:10:05
 * @Description  : letter-shell移植到ch582f的串口 1
 * @FilePath     : \CH32V203C8T6_LetterShell\CommonLibrary\letter-shell\src\shell_port.c
 */
#include "shell_port.h"
#include "bsp_usart.h"

/* 1. 创建shell对象，开辟shell缓冲区 */
Shell shell;
char shell_buffer[1024];

//void USARTx_SendByte(USART_TypeDef* pUSARTx, uint8_t data)
//{
//    USART_SendData(pUSARTx, data);
//    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
//}
//
//void USARTx_SendStr(USART_TypeDef* pUSARTx, char *str)
//{
//    uint8_t i = 0;
//    do
//    {
//       USARTx_SendByte(pUSARTx, *(str+i));
//       i++;
//    }while(*(str+i) != '\0');
//    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
//}

/* 2. 自己实现shell写函数 这里用了UART1*/
/**
 * @brief shell写数据函数原型
 *
 * @param data 需写的字符数据
 * @param len 需要写入的字符数
 *
 * @return unsigned short 实际写入的字符数量
 */
// typedef unsigned short (*shellWrite)(char *data, unsigned short len);
//__attribute__((used))
short LetterShell_Write(char *data, unsigned short len)
{
//    for(uint16_t i = 0; i < len; i++)
//    {
//        USART_SendData(USART1, *data++);
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
////        USART_SendData(USART1, *data++);
////        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//    }
//    USART_SendData(USART1, rev); //发送数据
//    USARTx_SendStr(USART1, data);
//    USART1_SendString(data);
//    lwrb_write(&usart1_tx_rb, data, len); /* Write data to TX buffer for loopback */
//    USART1_StartTxDMATransfer();              /* Then try to start transfer */

     USART1_SendArray(data, len);

//    while (len--)
//    {
//        /* 发送一个字节数据到USART */
//        USART_SendData(USART1, *(pdata++));
//
//        /* 等待发送数据寄存器为空 */
//        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//    }
//    /* 等待发送完成 */
//    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    return len;
}

// 对于裸机环境，在主循环中调用shellTask，或者在接收到数据时，调用shellHandler
// /**
//  * @brief 用户shell读
//  *
//  * @param data 数据
//  * @param len 数据长度
//  *
//  * @return short 实际读取到
//  */
//short LetterShell_Read(char *data, unsigned short len)
//{
//
//}

void uart1LogWrite(char *buffer, short len);

// 定义log对象
Log uart1Log = {
    .write = uart1LogWrite,
    .active = LOG_ENABLE,
    .level = LOG_ALL};

// 实现log写buffer函数
void uart1LogWrite(char *buffer, short len)
{
    if (uart1Log.shell)
    {
        //
        //        log工具可以结合letter shell的尾行模式，实现log和shell共用一个终端，但不影响shell交互体验
        shellWriteEndLine(uart1Log.shell, buffer, len);
    }
    //    UART1_SendString((uint8_t *)buffer, len);
}

/**
 * @description: 调用shellInit进行初始化
 * @return {*}
 */
void LetterShell_Init(void)
{
    shell.write = LetterShell_Write;

    // 调用shell初始化函数
    shellInit(&shell, shell_buffer, 1024);

    //    注册log对象
    logRegister(&uart1Log, &shell);

    // logDebug("LetterShell_Init OK!");
}
