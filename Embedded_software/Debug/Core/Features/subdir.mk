################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Features/am2320.c \
../Core/Features/comm.c \
../Core/Features/lm35dz.c \
../Core/Features/lps25hb.c 

OBJS += \
./Core/Features/am2320.o \
./Core/Features/comm.o \
./Core/Features/lm35dz.o \
./Core/Features/lps25hb.o 

C_DEPS += \
./Core/Features/am2320.d \
./Core/Features/comm.d \
./Core/Features/lm35dz.d \
./Core/Features/lps25hb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Features/%.o Core/Features/%.su: ../Core/Features/%.c Core/Features/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Core/Features -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Features

clean-Core-2f-Features:
	-$(RM) ./Core/Features/am2320.d ./Core/Features/am2320.o ./Core/Features/am2320.su ./Core/Features/comm.d ./Core/Features/comm.o ./Core/Features/comm.su ./Core/Features/lm35dz.d ./Core/Features/lm35dz.o ./Core/Features/lm35dz.su ./Core/Features/lps25hb.d ./Core/Features/lps25hb.o ./Core/Features/lps25hb.su

.PHONY: clean-Core-2f-Features

