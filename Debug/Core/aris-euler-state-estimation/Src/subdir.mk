################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/aris-euler-state-estimation/Src/env.c \
../Core/aris-euler-state-estimation/Src/flight_phase_detection.c \
../Core/aris-euler-state-estimation/Src/kf.c \
../Core/aris-euler-state-estimation/Src/state_est.c 

OBJS += \
./Core/aris-euler-state-estimation/Src/env.o \
./Core/aris-euler-state-estimation/Src/flight_phase_detection.o \
./Core/aris-euler-state-estimation/Src/kf.o \
./Core/aris-euler-state-estimation/Src/state_est.o 

C_DEPS += \
./Core/aris-euler-state-estimation/Src/env.d \
./Core/aris-euler-state-estimation/Src/flight_phase_detection.d \
./Core/aris-euler-state-estimation/Src/kf.d \
./Core/aris-euler-state-estimation/Src/state_est.d 


# Each subdirectory must supply rules for building sources it contributes
Core/aris-euler-state-estimation/Src/env.o: ../Core/aris-euler-state-estimation/Src/env.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/Users/linus/STM32IDE/aris-euler-recovery/Core/aris-euler-state-estimation/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/aris-euler-state-estimation/Src/env.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/aris-euler-state-estimation/Src/flight_phase_detection.o: ../Core/aris-euler-state-estimation/Src/flight_phase_detection.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/Users/linus/STM32IDE/aris-euler-recovery/Core/aris-euler-state-estimation/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/aris-euler-state-estimation/Src/flight_phase_detection.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/aris-euler-state-estimation/Src/kf.o: ../Core/aris-euler-state-estimation/Src/kf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/Users/linus/STM32IDE/aris-euler-recovery/Core/aris-euler-state-estimation/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/aris-euler-state-estimation/Src/kf.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/aris-euler-state-estimation/Src/state_est.o: ../Core/aris-euler-state-estimation/Src/state_est.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/Users/linus/STM32IDE/aris-euler-recovery/Core/aris-euler-state-estimation/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/aris-euler-state-estimation/Src/state_est.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

