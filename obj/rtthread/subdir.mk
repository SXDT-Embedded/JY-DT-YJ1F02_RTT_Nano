################################################################################
# MRS Version: {"version":"1.8.5","date":"2023/05/22"}
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rtthread/board.c 

OBJS += \
./rtthread/board.o 

C_DEPS += \
./rtthread/board.d 


# Each subdirectory must supply rules for building sources it contributes
rtthread/%.o: ../rtthread/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\Debug" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\include" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\components\drivers\include" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\components\drivers\include\drivers" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\components\drivers\include\ipc" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\components\drivers\include" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\components\drivers\include\drivers" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\components\drivers\include\ipc" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\drivers" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\components\device" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\components\finsh" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\include\libc" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\libcpu\risc-v\common" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\rtthread\include" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\Core" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\User" -I"C:\Users\yzy\Desktop\CH32V203C8T6_RTT_Nano\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

