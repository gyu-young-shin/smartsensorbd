################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Crc16.c \
../Src/Modbus_Crc.c \
../Src/MyTimer.c \
../Src/RS485.c \
../Src/RS485_2.c \
../Src/keyscan.c \
../Src/main.c \
../Src/stm32f0xx_hal_msp.c \
../Src/stm32f0xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f0xx.c 

OBJS += \
./Src/Crc16.o \
./Src/Modbus_Crc.o \
./Src/MyTimer.o \
./Src/RS485.o \
./Src/RS485_2.o \
./Src/keyscan.o \
./Src/main.o \
./Src/stm32f0xx_hal_msp.o \
./Src/stm32f0xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f0xx.o 

C_DEPS += \
./Src/Crc16.d \
./Src/Modbus_Crc.d \
./Src/MyTimer.d \
./Src/RS485.d \
./Src/RS485_2.d \
./Src/keyscan.d \
./Src/main.d \
./Src/stm32f0xx_hal_msp.d \
./Src/stm32f0xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/Crc16.cyclo ./Src/Crc16.d ./Src/Crc16.o ./Src/Crc16.su ./Src/Modbus_Crc.cyclo ./Src/Modbus_Crc.d ./Src/Modbus_Crc.o ./Src/Modbus_Crc.su ./Src/MyTimer.cyclo ./Src/MyTimer.d ./Src/MyTimer.o ./Src/MyTimer.su ./Src/RS485.cyclo ./Src/RS485.d ./Src/RS485.o ./Src/RS485.su ./Src/RS485_2.cyclo ./Src/RS485_2.d ./Src/RS485_2.o ./Src/RS485_2.su ./Src/keyscan.cyclo ./Src/keyscan.d ./Src/keyscan.o ./Src/keyscan.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/stm32f0xx_hal_msp.cyclo ./Src/stm32f0xx_hal_msp.d ./Src/stm32f0xx_hal_msp.o ./Src/stm32f0xx_hal_msp.su ./Src/stm32f0xx_it.cyclo ./Src/stm32f0xx_it.d ./Src/stm32f0xx_it.o ./Src/stm32f0xx_it.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32f0xx.cyclo ./Src/system_stm32f0xx.d ./Src/system_stm32f0xx.o ./Src/system_stm32f0xx.su

.PHONY: clean-Src

