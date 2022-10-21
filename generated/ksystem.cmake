add_definitions(-DF_CPU=16000000)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/library)

add_avr_library(ksystem STATIC
  ${CMAKE_CURRENT_LIST_DIR}/ksystem.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/demo/AVR/port/portevent.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/mb.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/rtu/mbrtu.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/functions/mbfunccoils.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/functions/mbfuncdiag.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/functions/mbfuncdisc.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/functions/mbfuncholding.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/functions/mbfuncinput.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/functions/mbfuncother.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/functions/mbutils.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus-avr-port/portserial.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus-avr-port/porttimer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus-avr-port/mbcrc.c
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/library/kModbus.cpp
)

avr_target_include_directories(ksystem PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus-avr-port
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/include
  ${CMAKE_CURRENT_LIST_DIR}/../libs/ksystem/third-party/freemodbus/modbus/rtu
)
