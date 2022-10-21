#pragma once

#include <stdint.h>

#include <avr/eeprom.h>

#include "config.h"
#include "settings.h"
#include "MovingAverage.h"
#include "CurrentTracker.h"

struct InputData // NOLINT(altera-struct-pack-align)
{
	CurrentTracker trackerForward, trackerBackward;
	MovingAverage<6> bus_mV;
	MovingAverage<6> power_mW;
};

namespace global
{
extern TConfig* currentConfiguration;

extern MovingAverage<6> shunt_mA;

extern InputData inputTotal;

#ifdef HAS_LOW_SIDE
extern InputData inputHigh;
extern InputData inputLow;
#endif
}
