cmake_minimum_required(VERSION 3.13...3.27)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico_w CACHE STRING "Board type")
include(lib/pico-sdk/pico_sdk_init.cmake)

include(lib/freeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)


project(pouch_two)

pico_sdk_init()

add_executable(pouch_two
    main.cpp
)

pico_enable_stdio_usb(pouch_two 1)
pico_enable_stdio_uart(pouch_two 0)

target_include_directories(pouch_two PRIVATE ${CMAKE_CURRENT_LIST_DIR} lib/)

target_link_libraries(pouch_two
    pico_cyw43_arch_lwip_threadsafe_background
    pico_stdlib
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

pico_add_extra_outputs(pouch_two)