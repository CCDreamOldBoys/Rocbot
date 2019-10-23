cmake_minimum_required(VERSION 3.5)
project(rocbot)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

add_definitions(
-DSTM32F40_41xxx
-DUSE_STDPERIPH_DRIVER
-DARM_MATH_CM4
)

#set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
set(MCU_FLAGS "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${MCU_FLAGS} -w -Wno-unknown-pragmas")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g2 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O3")

include_directories(
    src/sdk/stm32lib
    src/sdk/stm32lib/SYSTEM/delay
    src/sdk/stm32lib/SYSTEM/usart
    src/sdk/stm32lib/SYSTEM/sys
    src/sdk/stm32lib/inc
    src/sdk/stm32lib/CORE
    src/sdk/stm32lib/USB
    src/sdk/stm32lib/USB/STM32_USB_Device_Library/Class/cdc/inc
    src/sdk/stm32lib/USB/STM32_USB_Device_Library/Core/inc
    src/sdk/stm32lib/USB/STM32_USB_OTG_Driver/inc
    src/sdk/stm32lib/SDIO
    src/sdk/freertos/Source/include
    src/sdk/freertos/Source/portable/GCC/ARM_CM4F
    src/sdk/fatfs
    src/mavlink
    src/mavlink/common
    src/rocbot/control 
    src/rocbot/driver 
    src/rocbot/log 
    src/rocbot/math 
    src/rocbot/message 
    src/rocbot/module 
    src/rocbot/navigation 
    src/rocbot/sensor
    src/rocbot/system
    src/rocbot/task
    ${CMAKE_CURRENT_BINARY_DIR}
)

set_property(SOURCE src/sdk/stm32lib/startup/GCC/startup_stm32f40_41xxx.s PROPERTY LANGUAGE C)

add_library(stm32_lib
    src/sdk/stm32lib/startup/GCC/startup_stm32f40_41xxx.s
    src/sdk/stm32lib/src/misc.c 
    src/sdk/stm32lib/src/stm32f4xx_adc.c 
    src/sdk/stm32lib/src/stm32f4xx_can.c 
    src/sdk/stm32lib/src/stm32f4xx_dma.c 
    src/sdk/stm32lib/src/stm32f4xx_flash.c 
    src/sdk/stm32lib/src/stm32f4xx_rcc.c 
    src/sdk/stm32lib/src/stm32f4xx_gpio.c 
    src/sdk/stm32lib/src/stm32f4xx_tim.c 
    src/sdk/stm32lib/src/stm32f4xx_spi.c 
    src/sdk/stm32lib/src/stm32f4xx_pwr.c 
    src/sdk/stm32lib/src/stm32f4xx_sdio.c 
    src/sdk/stm32lib/src/stm32f4xx_usart.c 
    src/sdk/stm32lib/src/stm32f4xx_syscfg.c 
    src/sdk/stm32lib/system_stm32f4xx.c
    src/sdk/stm32lib/SYSTEM/sys/sys.c
    src/sdk/stm32lib/SYSTEM/usart/usart.c
    src/sdk/stm32lib/SYSTEM/delay/delay.c
    src/sdk/stm32lib/USB/usb_bsp.c 
    src/sdk/stm32lib/USB/usbd_desc.c
    src/sdk/stm32lib/USB/STM32_USB_Device_Library/Class/cdc/src/usbd_cdc_core.c
    src/sdk/stm32lib/USB/STM32_USB_Device_Library/Core/src/usbd_ioreq.c
    src/sdk/stm32lib/USB/STM32_USB_Device_Library/Core/src/usbd_req.c
    src/sdk/stm32lib/USB/STM32_USB_Device_Library/Core/src/usbd_core.c
    src/sdk/stm32lib/USB/STM32_USB_OTG_Driver/src/usb_core.c
    src/sdk/stm32lib/USB/STM32_USB_OTG_Driver/src/usb_dcd_int.c
    src/sdk/stm32lib/USB/STM32_USB_OTG_Driver/src/usb_dcd.c
)

add_library(freertos
    src/sdk/freertos/Source/tasks.c 
    src/sdk/freertos/Source/list.c 
    src/sdk/freertos/Source/queue.c 
    src/sdk/freertos/Source/portable/GCC/ARM_CM4F/port.c 
    src/sdk/freertos/Source/portable/MemMang/heap_4.c
)

add_library(fatfs
    src/sdk/fatfs/diskio.c 
    src/sdk/fatfs/ff.c 
    src/sdk/fatfs/option/ccsbcs.c
    src/sdk/stm32lib/SDIO/stm32f4_sdio_sd_LowLevel.c
    src/sdk/stm32lib/SDIO/stm32f4_sdio_sd.c
)
target_link_libraries(fatfs
    stm32_lib
)

file(GLOB SRC_DRIVER src/rocbot/driver/*.c)
file(GLOB SRC_CONTROL src/rocbot/control/*.c)
file(GLOB SRC_LOG src/rocbot/log/*.c)
file(GLOB SRC_MATH src/rocbot/math/*.c)
file(GLOB SRC_MESSAGE src/rocbot/message/*.c)
file(GLOB SRC_MODULE src/rocbot/module/*.c)
file(GLOB SRC_NAVIGATION src/rocbot/navigation/*.c)
file(GLOB SRC_SENSOR src/rocbot/sensor/*.c)
file(GLOB SRC_SYSTEM src/rocbot/system/*.c)
file(GLOB SRC_TASK src/rocbot/task/*.c)

add_library(${PROJECT_NAME}
    ${SRC_DRIVER}
    ${SRC_CONTROL}
    ${SRC_LOG}
    ${SRC_MATH}
    ${SRC_MESSAGE}
    ${SRC_MODULE}
    ${SRC_NAVIGATION}
    ${SRC_SENSOR}
    ${SRC_SYSTEM}
    ${SRC_TASK}
)
target_link_libraries(${PROJECT_NAME} -lm
    stm32_lib
    fatfs
    freertos
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/stm32lib/STM32F407_FLASH.ld)
set(CMAKE_EXE_LINKER_FLAGS
"--specs=nano.specs -specs=nosys.specs -nostartfiles -T${LINKER_SCRIPT} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref -Wl,--gc-sections"
)

add_executable(${PROJECT_NAME}.elf src/rocbot/main.c)
target_link_libraries(${PROJECT_NAME}.elf
    ${PROJECT_NAME}
)

set(ELF_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET "${PROJECT_NAME}.elf" POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Oihex  ${ELF_FILE} ${HEX_FILE}
    COMMENT "Building ${PROJECT_NAME}.bin and ${PROJECT_NAME}.hex"

    COMMAND ${CMAKE_COMMAND} -E copy ${HEX_FILE} "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex"
    COMMAND ${CMAKE_COMMAND} -E copy ${BIN_FILE} "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin"

    COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Invoking: Cross ARM GNU Print Size"
)
