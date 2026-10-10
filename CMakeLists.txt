cmake_minimum_required(VERSION 4.0)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(dux CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(DUX_SOURCES
        main.cpp
        include/dux/types.h
        kernel/printk.cpp
        include/dux/kernel/printk.h
        include/dux/kernel/console.h
        kernel/errno.cpp
        include/dux/irq_save_lock.h
        include/multiboot2/utils.h
        kernel/multiboot2/utils.cpp
        kernel/multiboot2/Parser.cpp
        include/multiboot2/Parser.h
)

if (ARCH STREQUAL "x86")
    list(APPEND DUX_SOURCES
            arch/x86/boot.S
            kernel/arch/x86/console.cpp
    )
endif ()


add_executable(${PROJECT_NAME} ${DUX_SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/arch/${ARCH}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dux
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libstdc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libstdc/include/arch/${ARCH}
        ${CMAKE_CURRENT_SOURCE_DIR}
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
        -static
        -no-pie
        -Wl,-T,${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/linker.ld
        -Wl,--gc-sections
)

add_subdirectory(lib)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        osdev-libcppabi
        osdev-libstdc
        gcc
)