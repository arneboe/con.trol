################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.c \
/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c 

OBJS += \
./Middlewares/USB_Device_Library/usbd_audio.o \
./Middlewares/USB_Device_Library/usbd_core.o \
./Middlewares/USB_Device_Library/usbd_ctlreq.o \
./Middlewares/USB_Device_Library/usbd_ioreq.o 

C_DEPS += \
./Middlewares/USB_Device_Library/usbd_audio.d \
./Middlewares/USB_Device_Library/usbd_core.d \
./Middlewares/USB_Device_Library/usbd_ctlreq.d \
./Middlewares/USB_Device_Library/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/USB_Device_Library/usbd_audio.o: /home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/USB_Device_Library/usbd_core.o: /home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/USB_Device_Library/usbd_ctlreq.o: /home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/USB_Device_Library/usbd_ioreq.o: /home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/arne/git/stm32-cubemx-usb-midi/test/Inc" -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include -I/home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.6.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


