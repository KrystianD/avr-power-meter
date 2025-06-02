#pragma once

#include <stdint.h>

#include <kEEPROM.h>

#include "ina228.h"

struct GlobalState
{
	float dieTemp;
	float vbus;
	float vshunt;
	float current;
	float power;
	float energy;
	float charge;

	int16_t raw_dieTemp;
	uint32_t raw_vbus;
	int32_t raw_vshunt;
	int32_t raw_current;
	int32_t raw_power;
	int64_t raw_energy;
	int64_t raw_charge;
};

#pragma pack(1)
struct Config
{
	uint8_t slaveId = 1;
	uint8_t baudRate = 0xff;
	uint16_t shuntRatedCurrent_A = 100;
	uint16_t shuntRatedVoltageDrop_mV = 75;
	ina228::Averages averages = ina228::Averages::Average16;
	ina228::ConversionTime conversionTime = ina228::ConversionTime::Conv50us;
};
#pragma pack()

namespace global
{
extern GlobalState state;
extern EEPROMMemory<Config> eeConfig;
}
