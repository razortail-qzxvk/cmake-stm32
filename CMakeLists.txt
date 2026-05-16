cmake_minimum_required(VERSION 3.28) # C++20 Modules 地表唯一支持 C++20 Modules 配交叉编译的元构建系统
project(stm32f407 ASM C CXX)
add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    core
)

target_sources(${PROJECT_NAME}
  PRIVATE
    startup_stm32f407xx.S
    core/retarget_io.cpp
    core/main.cpp
)

target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET CXX_MODULES FILES
    core/indicator.cppm
)

target_include_directories(${PROJECT_NAME}
  SYSTEM PRIVATE
    STM32CubeMX/Inc
    STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Inc
    STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    STM32CubeMX/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    STM32CubeMX/Drivers/CMSIS/Include
)

# SYSTEM PRIVATE 有助于抑制严格的用户代码警告, 作用在ARM或ST的实现上.
# 而且它会把 -I 提升为 -isystem, 减轻 clangd 的压力.

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
    STM32CubeMX/Src/sysmem.c
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
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -gdwarf-4

    $<$<COMPILE_LANGUAGE:C,CXX>:
      -O0
      -Wall -Wextra -Wpedantic
      # -Werror
      -ffunction-sections -fdata-sections
      -fstack-usage
      -funsigned-char
      -Wno-packed
      -Wno-reserved-id-macro
      -Wno-unused-macros
      -Wno-documentation-unknown-command
      -Wno-documentation
      -Wno-parentheses-equality
      -Wno-reserved-identifier
    >

    $<$<COMPILE_LANGUAGE:CXX>:
      -fno-exceptions -fno-rtti
      -fno-threadsafe-statics
      -fno-use-cxa-atexit
    >
)

# -Wall -Wextra -Wpedantic -Werror 警告
# -fno-exceptions -fno-rtti 禁用异常
# -ffunction-sections -fdata-sections 配合 -Wl,--gc-sections 剔除未使用的函数和数据
# -fstack-usage 生成 .su 文件, 记录栈占用.
# -funsigned-char ARM特色
# -gdwarf-4 调试兼容选项
# -Wno* CMSIS和ST非标准代码需要
# -fno-threadsafe-statics 局部static变量不再线程安全
# -fno-use-cxa-atexit 移除程序退出时的析构代码


target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    $<$<COMPILE_LANGUAGE:C,CXX>:
      STM32F407xx
      USE_HAL_DRIVER
      USE_FULL_LL_DRIVER
      CMSIS_device_header="stm32f407xx.h"
    >
)

# target_compile_features(${PROJECT_NAME} PRIVATE c_std_11 cxx_std_26)
set_target_properties(${PROJECT_NAME} PROPERTIES
  C_STANDARD 11
  C_STANDARD_REQUIRED YES
  C_EXTENSIONS OFF
  CXX_STANDARD 26
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
)

target_link_options(${PROJECT_NAME}
  PRIVATE
    "-T${CMAKE_CURRENT_SOURCE_DIR}/STM32F407xx_FLASH.ld"
    "-nostartfiles"
    "-Wl,-Map=${PROJECT_NAME}.map"
    "-Wl,--cref"
    "-Wl,--print-memory-usage"
    "-Wl,--gc-sections"
    "-Wl,--build-id=none"
    "-Wl,-z,noexecstack"
)

# -nostartfiles 可以不写. 不使用默认启动文件(比如crt0.o), 但CMake可以不写.
# -Wl,--cref -Wl,-Map=${PROJECT_NAME}.map 配合 生成交叉引用表
# -Wl,--print-memory-usage 链接后打印内存使用情况
# -Wl,--build-id=none 禁用构建标识符, 嵌入式不需要
# -Wl,-z,noexecstack 现代LLVM需要

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
  COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:${PROJECT_NAME}> > ${PROJECT_NAME}.lst
  # COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}>
  VERBATIM
)

# .lst 全景反汇编列表
# --all-headers 打印所有的 ELF 段（Section）头信息
# --demangle C++ 符号解混淆
# --disassemble 反汇编代码段

# CMAKE_SIZE
# 可以不写
# --format=berkeley 输出格式

# VERBATIM 确保命令字符串按字面意思处理, 不进行 CMake 变量替换等

# 补丁
# 通常 -isystem 可以抑制头文件的非标准代码, 但.c就无能为力了.
set_source_files_properties(
  STM32CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
  PROPERTIES
    COMPILE_OPTIONS "-Wno-unused-parameter"
)
