#include <kSerial.h>
#include <kSysTick.h>
#include <kSysTimer.h>
#include <kGPIO.h>
#include <kModbus.h>

#pragma once
// Component: SerialComponent
static kSerial Serial;

// Component: SysTickComponent
typedef kSysTick<0,125> kConfiguredSysTick;
typedef kConfiguredSysTick SysTick;
extern void readTimerElapsed();
extern kSysTimer<1,nullptr,true> readTimer;
extern void modbusTimerElapsed();
extern kSysTimer<1,modbusTimerElapsed,false> modbusTimer;

// Component: GPIOComponent
#pragma push_macro("_SFR_IO8")
#undef _SFR_IO8
#define _SFR_IO8(x) x
static kGPIO<PORTC,0> led;
static kGPIO<PORTD,2> dir;
static kGPIO<PORTC,3> addr1;
static kGPIO<PORTC,2> addr2;
static kGPIO<PORTC,1> addr4;
#pragma pop_macro("_SFR_IO8")

// Component: ModbusComponent
static kModbus Modbus;
#define MODBUS_ENABLED 1
