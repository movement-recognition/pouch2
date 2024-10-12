cmake_minimum_required(VERSION 3.13...3.27)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(PICO_BOARD pico_w CACHE STRING "Board type")
include(lib/pico-sdk/pico_sdk_init.cmake)

project(pouch2)

pico_sdk_init()

add_executable(pouch2
    main.cpp
)

target_link_libraries(pouch2 pico_stdlib)

pico_add_extra_outputs(pouch2)