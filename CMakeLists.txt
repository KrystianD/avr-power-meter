cmake_minimum_required(VERSION 3.15)
project(avr-power-meter-v3)

set(CMAKE_CXX_STANDARD 17)
set(ATTINY_DEVICE attiny1624)

include(libs/dev_attiny/attiny.cmake)
include(generated/ksystem.cmake)

include_directories(.)

add_avr_executable(main
        src/main.cpp
        src/i2c.cpp
        src/modbus.cpp
        src/ina228.cpp
        src/global.cpp
)
avr_target_link_libraries(main ksystem)
