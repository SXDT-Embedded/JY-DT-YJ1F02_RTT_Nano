################################################################################
# MRS Version: {"version":"1.8.5","date":"2023/05/22"}
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/drv_gpio.c \
../drivers/drv_usart.c 

OBJS += \
./drivers/drv_gpio.o \
./drivers/drv_usart.o 

C_DEPS += \
./drivers/drv_gpio.d \
./drivers/drv_usart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\bsp\inc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\ipc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\ipc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\ipc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\device" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\finsh" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\include\libc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\libcpu\risc-v\common" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\Core" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\User" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\Peripheral\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

