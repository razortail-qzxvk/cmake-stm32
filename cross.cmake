
# 交叉编译
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
# 跳过链接测试
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
# 链接为elf后缀
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)
set(CMAKE_EXECUTABLE_SUFFIX_C   .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
# ATfE & Ninja
set(toolchain_path "C:/ATfE-22.1.0-Windows-x86_64")
set(CMAKE_C_COMPILER "${toolchain_path}/bin/clang.exe")
set(CMAKE_MAKE_PROGRAM "C:/Ninja/ninja.exe" CACHE FILEPATH "")
set(CMAKE_FIND_ROOT_PATH ${toolchain_path})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
unset(toolchain_path)
# --target=arm-none-eabi
set(CMAKE_ASM_COMPILER_TARGET arm-none-eabi)
set(CMAKE_C_COMPILER_TARGET   arm-none-eabi)
set(CMAKE_CXX_COMPILER_TARGET arm-none-eabi)
# 全局编译选项
set(core_flags "")
string(APPEND core_flags "-freestanding ") # 可以不写; 加上之后main函数会变为普通函数(需要extern "C"给汇编)
string(APPEND core_flags "--config=llvmlibc.cfg ")
string(APPEND core_flags "-mcpu=cortex-m4 ")
string(APPEND core_flags "-mfloat-abi=hard ")
string(APPEND core_flags "-mfpu=fpv4-sp-d16 ")
set(CMAKE_ASM_FLAGS_INIT "${core_flags}")
set(CMAKE_C_FLAGS_INIT   "${core_flags} -rtlib=compile-rt") # LLVM原汁原味
set(CMAKE_CXX_FLAGS_INIT "${core_flags} -stdlib=libc++") # LLVM原汁原味
unset(core_flags)
# compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# objcopy 可以不指定. 提供把 elf 文件转换成其他东西的能力.
# size 可以不指定. 提供查看 elf 文件大小的能力.

# --config=llvmlibc.cfg
# 可以不写.
# 这是一个非常深入的选项.
# 首先, 对于GCC来说, newlib 来源于硬编码嗅探.
# 现代 LLVM, 采用 .cfg + .yaml 的方式来重构.
# llvmlibc.cfg 负责写清楚 --sysroot (所以你在构建配置里就不建议再写了), 以及通知 clang 开启Mutilib支持机制.
# clang 接着会去 sysroot 找到一个名为 mutilib.yaml 的文件, 这个文件会**捕获**你的编译参数(-mcpu -mfpu -mfloat-abi等), 匹配之后再行追加其他参数.
# 对于 CMake 来说, 这个新时代的嗅探机制和它本来的疯狗嗅探是冲突的, 所以你得限制死 CMAKE_FIND_ROOT_PATH_MODE_xxx.
# Meson 则是正常的将 --config=llvmlibc.cfg 传递给 clang 就行.