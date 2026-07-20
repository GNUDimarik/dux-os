cmake_minimum_required(VERSION 4.0)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(dux CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(dux main.cpp
        include/dux/system/device/IDevice.h
        include/dux/system/device/ICharDevice.h
        include/dux/system/device/IBlockDevice.h
        include/dux/types.h
        include/arch/x86/asm.h
        kernel/printk.cpp
        include/dux/kernel/printk.h
        include/dux/kernel/console.h
        kernel/arch/x86/early_console.cpp
        include/dux/system/device/video/IConsole.h
        drivers/char/video/vga/text/x86/EarlyConsole.cpp
        drivers/char/video/vga/text/x86/EarlyConsole.h
        include/dux/system/device/AbstractDevice.h
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        lib/libstdc/include
)

target_compile_options(${PROJECT_NAME}
        PRIVATE
        -Wall
        -ffreestanding
        -fno-exceptions
        -fno-rtti
        -fno-stack-protector
        -nostdlib
        -std=gnu++20
)

target_link_options(${PROJECT_NAME}
        PRIVATE
        -nostdlib
        -nodefaultlibs
)

add_subdirectory(lib)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        osdev-libcppabi
        osdev-libstdc
        gcc
)