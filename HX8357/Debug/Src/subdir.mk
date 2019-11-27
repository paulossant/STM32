################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/9481.c \
../Src/fonts.c \
../Src/main.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f1xx.c \
../Src/touch.c 

OBJS += \
./Src/9481.o \
./Src/fonts.o \
./Src/main.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f1xx.o \
./Src/touch.o 

C_DEPS += \
./Src/9481.d \
./Src/fonts.d \
./Src/main.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f1xx.d \
./Src/touch.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/media/pssantos/HDD/STM32/LCD_35inch/Inc" -I"/media/pssantos/HDD/STM32/LCD_35inch/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/media/pssantos/HDD/STM32/LCD_35inch/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/media/pssantos/HDD/STM32/LCD_35inch/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/media/pssantos/HDD/STM32/LCD_35inch/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


