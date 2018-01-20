################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/arne/git/stm32-cubemx-usb-midi/test/Src/main.c \
/home/arne/git/stm32-cubemx-usb-midi/test/Src/stm32f1xx_hal_msp.c \
/home/arne/git/stm32-cubemx-usb-midi/test/Src/stm32f1xx_it.c \
/home/arne/git/stm32-cubemx-usb-midi/test/Src/usb_device.c \
/home/arne/git/stm32-cubemx-usb-midi/test/Src/usbd_audio_if.c \
/home/arne/git/stm32-cubemx-usb-midi/test/Src/usbd_conf.c \
/home/arne/git/stm32-cubemx-usb-midi/test/Src/usbd_desc.c 

OBJS += \
./Application/User/main.o \
./Application/User/stm32f1xx_hal_msp.o \
./Application/User/stm32f1xx_it.o \
./Application/User/usb_device.o \
./Application/User/usbd_audio_if.o \
./Application/User/usbd_conf.o \
./Application/User/usbd_desc.o 

C_DEPS += \
./Application/User/main.d \
./Application/User/stm32f1xx_hal_msp.d \
./Application/User/stm32f1xx_it.d \
./Application/User/usb_device.d \
./Application/User/usbd_audio_if.d \
./Application/User/usbd_conf.d \
./Application/User/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/main.o: /home/arne/git/stm32-cubemx-usb-midi/test/Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f1xx_hal_msp.o: /home/arne/git/stm32-cubemx-usb-midi/test/Src/stm32f1xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f1xx_it.o: /home/arne/git/stm32-cubemx-usb-midi/test/Src/stm32f1xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/usb_device.o: /home/arne/git/stm32-cubemx-usb-midi/test/Src/usb_device.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/usbd_audio_if.o: /home/arne/git/stm32-cubemx-usb-midi/test/Src/usbd_audio_if.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/usbd_conf.o: /home/arne/git/stm32-cubemx-usb-midi/test/Src/usbd_conf.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/usbd_desc.o: /home/arne/git/stm32-cubemx-usb-midi/test/Src/usbd_desc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


