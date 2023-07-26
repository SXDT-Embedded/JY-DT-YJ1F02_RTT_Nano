################################################################################
# MRS Version: {"version":"1.8.5","date":"2023/05/22"}
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/src/bsp_buzzer.c \
../bsp/src/bsp_led.c 

OBJS += \
./bsp/src/bsp_buzzer.o \
./bsp/src/bsp_led.o 

C_DEPS += \
./bsp/src/bsp_buzzer.d \
./bsp/src/bsp_led.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/src/%.o: ../bsp/src/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\bsp\inc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\ipc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\ipc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\ipc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\device" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\finsh" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\include\libc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\libcpu\risc-v\common" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\Core" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\User" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\Peripheral\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

