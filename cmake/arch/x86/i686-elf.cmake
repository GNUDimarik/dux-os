set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(CMAKE_C_COMPILER   "$ENV{HOME}/opt/cross/bin/i686-elf-gcc")
set(CMAKE_CXX_COMPILER "$ENV{HOME}/opt/cross/bin/i686-elf-g++")
set(CMAKE_ASM_COMPILER "$ENV{HOME}/opt/cross/bin/i686-elf-gcc")

# CMake должен только компилировать тестовый объект,
# а не пытаться собрать hosted executable с libc и crt0.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Не искать библиотеки и заголовки хостовой Linux-системы.
set(CMAKE_FIND_ROOT_PATH "$ENV{HOME}/opt/cross/i686-elf")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)