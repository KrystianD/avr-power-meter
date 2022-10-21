project(avr-power-meter-v2)
cmake_minimum_required(VERSION 3.15)

set(AVR_PROGRAMMER usbasp)

set(CMAKE_CXX_STANDARD 17)

include(cmake/avr.cmake)
include(generated/ksystem.cmake)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder -fpermissive")
set(CMAKE_CXX_FLAGS_DEBUG "-Os")
set(CMAKE_CXX_FLAGS_RELEASE "-Os")

include_directories(.)
include_directories(libs/ksystem/library)
include_directories(libs/avr-eeprom-wear-leveling/include)

add_avr_executable(
        main
        src/main.cpp
        src/state.cpp
        src/modbus.cpp
        src/i2c.cpp
        src/storage.cpp
        src/settings.cpp
        libs/avr-eeprom-wear-leveling/src/eeprom_async.cpp
)
avr_target_link_libraries(main ksystem)
