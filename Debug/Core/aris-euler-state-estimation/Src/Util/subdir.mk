################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/aris-euler-state-estimation/Src/Util/math_utils.c 

OBJS += \
./Core/aris-euler-state-estimation/Src/Util/math_utils.o 

C_DEPS += \
./Core/aris-euler-state-estimation/Src/Util/math_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/aris-euler-state-estimation/Src/Util/math_utils.o: ../Core/aris-euler-state-estimation/Src/Util/math_utils.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/Users/linus/STM32IDE/aris-euler-recovery/Core/aris-euler-state-estimation/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/aris-euler-state-estimation/Src/Util/math_utils.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

