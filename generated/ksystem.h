#pragma once

#include "kdevice.h"
#include <kSerial.h>
#include <kGPIO.h>
#include <kSerial.h>
#include <kGPIO.h>
#include <kSysTick.h>
#include <stdio.h>
#include <kGPIO.h>
#include <kModbus.h>

// Component: InterruptsComponent

// Component: SerialComponent
static kSerial<kUSART0> debug;

// Component: SerialComponent
static kSerial<kUSART1> modbus;

// Component: SysTickComponent
typedef kSysTick<33> kConfiguredSysTick;
extern kConfiguredSysTick SysTick;

// Component: StdioComponent

// Component: GPIOComponent
static kGPIO<kPORTB,0> scl;
static kGPIO<kPORTB,1> sda;
static kGPIO<kPORTB,3> alert;
static kGPIO<kPORTA,7> led;

// Component: ModbusComponent
static kModbus Modbus;
#define MODBUS_ENABLED 1
