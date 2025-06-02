#pragma once

#include <stdint.h>

namespace ina228
{
enum class Mode : uint8_t
{
	Shutdown = 0,
	TriggeredBus = 1,
	TriggeredShunt = 2,
	TriggeredBusShunt = 3,
	TriggeredTemperature = 4,
	TriggeredTemperatureBus = 5,
	TriggeredTemperatureShunt = 6,
	TriggeredTemperatureBusShunt = 7,

	Shutdown2 = 8 + 0,
	ContinuousBus = 8 + 1,
	ContinuousShunt = 8 + 2,
	ContinuousBusShunt = 8 + 3,
	ContinuousTemperature = 8 + 4,
	ContinuousTemperatureBus = 8 + 5,
	ContinuousTemperatureShunt = 8 + 6,
	ContinuousTemperatureBusShunt = 8 + 7,
};

enum class Averages : uint8_t
{
	Min = 0,
	Average1 = 0,
	Average4 = 1,
	Average16 = 2,
	Average64 = 3,
	Average128 = 4,
	Average256 = 5,
	Average512 = 6,
	Average1024 = 7,
	Max = Average1024,
};

enum class ConversionTime : uint8_t
{
	Min = 0,
	Conv50us = 0,
	Conv84us = 1,
	Conv150us = 2,
	Conv280us = 3,
	Conv540us = 4,
	Conv1052us = 5,
	Conv2074us = 6,
	Conv4120us = 7,
	Max = Conv4120us,
};

enum class AdcRange : uint8_t
{
	Scale163_84mV = 0,
	Scale40_96mV = 1,
};

enum class Registers : uint8_t
{
	CONFIG = 0,
	ADC_CONFIG = 1,
	SHUNT_CAL = 2,
	VSHUNT = 4,
	VBUS = 5,
	DIETEMP = 6,
	CURRENT = 7,
	POWER = 0x08,
	ENERGY = 0x09,
	CHARGE = 0x0a,
	DIAG_ALERT = 0x0b,
};

#pragma pack(1)
struct Configuration
{
	int reserved : 4;
	AdcRange adcRange : 1;
	bool tempCompensation : 1;
	int conversionDelay : 8;
	bool resetAccumulators : 1;
	bool reset : 1;
};

struct AdcConfiguration
{
	Averages avg : 3;
	ConversionTime vtempct : 3;
	ConversionTime vshuntct : 3;
	ConversionTime vbusct : 3;
	Mode mode : 4;
};
#pragma pack()

bool writeRegister16(Registers reg, uint16_t value);
bool readRegister16(Registers reg, int32_t* value);
bool readRegister24(Registers reg, int32_t* value);
bool readRegister40(Registers reg, uint64_t* value);
bool readRegister40s(Registers reg, int64_t* value);

bool writeConfig(const Configuration& config);
bool writeAdcConfig(const AdcConfiguration& config);
bool clearStatus();

bool readDieTempValue(int16_t& val);
bool readVbusValue(uint32_t& val);
bool readVshuntValue(int32_t& val);
bool readCurrentValue(int32_t& val);
bool readPowerValue(int32_t& val);
bool readEnergyValue(int64_t& val);
bool readChargeValue(int64_t& val);
}
