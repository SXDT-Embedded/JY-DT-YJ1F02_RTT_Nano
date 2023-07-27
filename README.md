# JT-DT-YD1C01_RTT_Nano

#### 介绍

基础物联款家用可燃气体探测器（甲烷）JT-DT-YD1C01 裸机框架移植到RTT Nano 3.1.5

## RTT移植注意事项

> 参考WCH官方的文档[RISC-V 平台移植 RTOS]()

即在 MRS 声明 中断函数时由 __attribute__((interrupt("WCH Interrupt fast 方式 定义 编译器
不 自动添加 软件出入栈代码 ，由 __ attribute__(( 方式定义 编译器 添加 软件出 入栈的
代码
