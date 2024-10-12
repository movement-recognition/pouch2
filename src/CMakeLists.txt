cmake_minimum_required(VERSION 3.13...3.27)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(PICO_BOARD pico_w CACHE STRING "Board type")
include(lib/pico-sdk/pico_sdk_init.cmake)

project(pouch_two)

pico_sdk_init()

add_executable(pouch_two
    main.cpp
)

pico_enable_stdio_usb(pouch_two 1)
pico_enable_stdio_uart(pouch_two 0)

target_link_libraries(pouch_two pico_stdlib)

pico_add_extra_outputs(pouch_two)