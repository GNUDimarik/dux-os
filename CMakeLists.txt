cmake_minimum_required(VERSION 4.0)
project(dux)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(dux main.cpp
        include/dux/system/device/IDevice.h
        include/dux/system/device/ICharacterDevice.h
        include/dux/system/device/IBlockDevice.h
        include/dux/types.h
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
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
)