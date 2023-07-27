/*
 * @Author       : yzy
 * @Date         : 2022-11-24 13:24:59
 * @LastEditors  : yzy
 * @LastEditTime : 2022-11-24 13:51:54
 * @Description  : letter-shell移植到ch582f的串口 1
 * @FilePath     : \vscode_ch582_test\CommonLibrary\letter-shell\shell_port.h
 */
#ifndef __SHELL_PORT_H__
#define __SHELL_PORT_H__

#include "ch32v20x.h"
#include "shell.h"
#include "log.h"

/* 将shell定义为外部变量，在串口中断回调函数中还要使用 */
extern Shell shell;
extern Log uart1Log;

/**
 * @description: 调用shellInit进行初始化
 * @return {*}
 */
void LetterShell_Init(void);


#endif /* __SHELL_PORT_H__ */
