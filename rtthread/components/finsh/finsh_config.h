/* FinSH config file */

#ifndef __MSH_CFG_H__
#define __MSH_CFG_H__

/* ���� FinSH */
#define RT_USING_FINSH

/* ���߳����ƶ���Ϊ tshell */
#define FINSH_THREAD_NAME "tshell"

/* ������ʷ���� */
#define FINSH_USING_HISTORY
/* ��¼ 5 ����ʷ���� */
#define FINSH_HISTORY_LINES 5

/* ����ʹ�� Tab �� */
#define FINSH_USING_SYMTAB
/* ������������ */
#define FINSH_USING_DESCRIPTION

/* ���� FinSH �߳����ȼ�Ϊ  */
#define __FINSH_THREAD_PRIORITY     5
#define FINSH_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX / 8 * __FINSH_THREAD_PRIORITY + 1)

/* ���� FinSH �̵߳�ջ��СΪ  */
#define FINSH_THREAD_STACK_SIZE     1024

/* ���������ַ�����Ϊ 80 �ֽ� */
#define FINSH_CMD_SIZE 80

/* ���� msh ���� */
#define FINSH_USING_MSH
#define FINSH_USING_MSH_ONLY
/* ��������������Ϊ 10 �� */
#define FINSH_ARG_MAX 3




#endif //!__MSH_CFG_H__

