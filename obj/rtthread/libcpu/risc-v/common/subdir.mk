################################################################################
# MRS Version: {"version":"1.8.5","date":"2023/05/22"}
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rtthread/libcpu/risc-v/common/cpuport.c 

S_UPPER_SRCS += \
../rtthread/libcpu/risc-v/common/context_gcc.S \
../rtthread/libcpu/risc-v/common/interrupt_gcc.S 

OBJS += \
./rtthread/libcpu/risc-v/common/context_gcc.o \
./rtthread/libcpu/risc-v/common/cpuport.o \
./rtthread/libcpu/risc-v/common/interrupt_gcc.o 

S_UPPER_DEPS += \
./rtthread/libcpu/risc-v/common/context_gcc.d \
./rtthread/libcpu/risc-v/common/interrupt_gcc.d 

C_DEPS += \
./rtthread/libcpu/risc-v/common/cpuport.d 


# Each subdirectory must supply rules for building sources it contributes
rtthread/libcpu/risc-v/common/%.o: ../rtthread/libcpu/risc-v/common/%.S
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
rtthread/libcpu/risc-v/common/%.o: ../rtthread/libcpu/risc-v/common/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\bsp\inc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\ipc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\drivers\include\ipc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\drivers" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\device" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\components\finsh" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\include\libc" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\libcpu\risc-v\common" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\rtthread\include" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\Core" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\User" -I"E:\BaiduSyncdisk\SXDT\Household_gas_alarm\Base_Iot\JT-DT-YD1C01_RTT_Nano\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

