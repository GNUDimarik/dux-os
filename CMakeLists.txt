cmake_minimum_required(VERSION 4.0)
project(dux)

set(CMAKE_CXX_STANDARD 20)

add_executable(dux main.cpp
        include/dux/system/device/IDevice.h
        include/dux/system/device/ICharacterDevice.h
        include/dux/system/device/IBlockDevice.h
        include/dux/types.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)