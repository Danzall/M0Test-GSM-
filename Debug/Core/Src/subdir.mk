################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Flash_Driver.c \
../Core/Src/GSM.c \
../Core/Src/MQTT.c \
../Core/Src/Relay.c \
../Core/Src/Wifi.c \
../Core/Src/bootloader.c \
../Core/Src/freertos.c \
../Core/Src/ggFSM.c \
../Core/Src/main.c \
../Core/Src/myString.c \
../Core/Src/stm32f0xx_hal_msp.c \
../Core/Src/stm32f0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f0xx.c 

OBJS += \
./Core/Src/Flash_Driver.o \
./Core/Src/GSM.o \
./Core/Src/MQTT.o \
./Core/Src/Relay.o \
./Core/Src/Wifi.o \
./Core/Src/bootloader.o \
./Core/Src/freertos.o \
./Core/Src/ggFSM.o \
./Core/Src/main.o \
./Core/Src/myString.o \
./Core/Src/stm32f0xx_hal_msp.o \
./Core/Src/stm32f0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f0xx.o 

C_DEPS += \
./Core/Src/Flash_Driver.d \
./Core/Src/GSM.d \
./Core/Src/MQTT.d \
./Core/Src/Relay.d \
./Core/Src/Wifi.d \
./Core/Src/bootloader.d \
./Core/Src/freertos.d \
./Core/Src/ggFSM.d \
./Core/Src/main.d \
./Core/Src/myString.d \
./Core/Src/stm32f0xx_hal_msp.d \
./Core/Src/stm32f0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

