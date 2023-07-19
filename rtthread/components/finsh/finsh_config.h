/* FinSH config file */

#ifndef __MSH_CFG_H__
#define __MSH_CFG_H__

/* 开启 FinSH */
#define RT_USING_FINSH

/* 将线程名称定义为 tshell */
#define FINSH_THREAD_NAME "tshell"

/* 开启历史命令 */
#define FINSH_USING_HISTORY
/* 记录 5 行历史命令 */
#define FINSH_HISTORY_LINES 5

/* 开启使用 Tab 键 */
#define FINSH_USING_SYMTAB
/* 开启描述功能 */
#define FINSH_USING_DESCRIPTION

/* 定义 FinSH 线程优先级为  */
#define __FINSH_THREAD_PRIORITY     5
#define FINSH_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX / 8 * __FINSH_THREAD_PRIORITY + 1)

/* 定义 FinSH 线程的栈大小为  */
#define FINSH_THREAD_STACK_SIZE     1024

/* 定义命令字符长度为 80 字节 */
#define FINSH_CMD_SIZE 80

/* 开启 msh 功能 */
#define FINSH_USING_MSH
#define FINSH_USING_MSH_ONLY
/* 最大输入参数数量为 10 个 */
#define FINSH_ARG_MAX 3




#endif //!__MSH_CFG_H__

