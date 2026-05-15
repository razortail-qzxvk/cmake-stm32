cmake_minimum_required(VERSION 3.28) # C++20 Modules 地表唯一支持 C++20 Modules 配交叉编译的元构建系统
project(cmake_stm32 ASM C CXX)
add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    core
)

target_sources(${PROJECT_NAME}
  PRIVATE
    core/main.cpp
)

target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET CXX_MODULES FILES
    core/test.cppm
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    STM32CubeMX/Inc
    STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Inc
    STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    STM32CubeMX/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    STM32CubeMX/Drivers/CMSIS/Include
)

target_sources(${PROJECT_NAME}
  PRIVATE
    STM32CubeMX/Src/main.c
	  STM32CubeMX/Src/gpio.c
	  STM32CubeMX/Src/dma.c
	  STM32CubeMX/Src/spi.c
	  STM32CubeMX/Src/tim.c
	  STM32CubeMX/Src/usart.c
	  STM32CubeMX/Src/stm32f4xx_it.c
	  STM32CubeMX/Src/stm32f4xx_hal_msp.c
	  STM32CubeMX/Src/system_stm32f4xx.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
	  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
	  STM32CubeMX/startup_stm32f407xx.s
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -O0
    -Wall -Wextra -Wpedantic -Werror
    -fno-exceptions -fno-rtti
    -ffunction-sections -fdata-sections
    -fstack-usage
    -funsigned-char
    -gdwarf-4
    -Wno-packed
    -Wno-reserved-id-macro
    -Wno-unused-macros
    -Wno-documentation-unknown-command
    -Wno-documentation
    -Wno-license-management
    -Wno-parentheses-equality
    -Wno-reserved-identifier 
)

# -Wall -Wextra -Wpedantic -Werror 警告
# -fno-exceptions -fno-rtti 禁用异常
# -ffunction-sections -fdata-sections 配合 -Wl,--gc-sections 剔除未使用的函数和数据
# -fstack-usage 生成 .su 文件, 记录栈占用.
# -funsigned-char ARM特色
# -gdwarf-4 调试兼容选项
# -Wno* CMSIS和ST非标准代码需要


target_compile_definitions(${PROJECT_NAME} PRIVATE
    STM32F407xx
    USE_FULL_LL_DRIVER
    CMSIS_device_header="stm32f407xx.h"
)

target_compile_features(${PROJECT_NAME}
  PRIVATE
    c_std_11
    cxx_std_26
)

target_link_options(${PROJECT_NAME}
  PRIVATE
    "-T${CMAKE_CURRENT_SOURCE_DIR}/STM32F407xx_FLASH.ld"
    "-nostartfiles"
    "-Wl,-Map=${PROJECT_NAME}.map"
    "-Wl,--print-memory-usage"
    "-Wl,--gc-sections"
    "-Wl,-z,noexecstack"
)

# -nostartfiles 可以不写. 不使用默认启动文件(比如crt0.o), 但CMake可以不写.
# -Wl,--print-memory-usage 链接后打印内存使用情况
# -Wl,-z,noexecstack 现代LLVM需要
