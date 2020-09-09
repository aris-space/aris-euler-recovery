################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/devices/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
aris-euler-recovery.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
aris-euler-recovery.list \

OBJCOPY_BIN += \
aris-euler-recovery.bin \


# All Target
all: aris-euler-recovery.elf secondary-outputs

# Tool invocations
aris-euler-recovery.elf: $(OBJS) $(USER_OBJS) /Users/linus/STM32IDE/aris-euler-recovery/STM32F405VGTX_FLASH.ld
	arm-none-eabi-gcc -o "aris-euler-recovery.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/linus/STM32IDE/aris-euler-recovery/STM32F405VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="aris-euler-recovery.map" -Wl,--gc-sections -static -L/Users/linus/STM32IDE/aris-euler-recovery/Core/aris-euler-state-estimation/Inc --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

aris-euler-recovery.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "aris-euler-recovery.list"
	@echo 'Finished building: $@'
	@echo ' '

aris-euler-recovery.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "aris-euler-recovery.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
