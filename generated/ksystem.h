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
static kGPIO<PORTD,4> led;
static kGPIO<PORTB,0> out3;
static kGPIO<PORTB,1> out2;
static kGPIO<PORTB,2> out1;
static kGPIO<PORTD,2> dir;
static kGPIO<PORTB,2> servo;
#pragma pop_macro("_SFR_IO8")

// Component: ModbusComponent
static kModbus Modbus;
#define MODBUS_ENABLED 1
