################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/arne/git/stm32-cubemx-usb-midi/test/Src/system_stm32f1xx.c 

OBJS += \
./Drivers/CMSIS/system_stm32f1xx.o 

C_DEPS += \
./Drivers/CMSIS/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/system_stm32f1xx.o: /home/arne/git/stm32-cubemx-usb-midi/test/Src/system_stm32f1xx.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


