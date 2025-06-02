#include "ina228.h"

#include <ksystem.h>

#include "i2c.h"

namespace ina228
{
constexpr uint8_t INA228_ADDRESS = 0x40;

bool writeRegister16(Registers reg, uint16_t value)
{
	uint8_t wr[3];
	wr[0] = (uint8_t)reg;
	wr[1] = value >> 8;
	wr[2] = value;

	if (!i2cTx(INA228_ADDRESS, wr, 3, nullptr, 0))
		return false;

	return true;
}

bool readRegister16(Registers reg, int16_t* value)
{
	if (!i2cTx(INA228_ADDRESS, (uint8_t*)&reg, 1, (uint8_t*)value, 2))
		return false;

	*value = (int16_t)__builtin_bswap16(*value);

	return true;
}

bool readRegister24(Registers reg, int32_t* value)
{
	*value = 0;
	if (!i2cTx(INA228_ADDRESS, (uint8_t*)&reg, 1, (uint8_t*)value, 3))
		return false;

	/*
	 * | 0 | 1 | 2 | 3 |
	 * | H | M | L | 0 | - value
	 */

	*value = (int32_t)__builtin_bswap32(*value) >> 8;
	/*
	 * | 0 | 1 | 2 | 3 |
	 * | H | M | L | 0 | - value
	 * | 0 | L | M | H | - after swap
	 * | L | M | H | 0 | - after swap and shift
	 */

	return true;
}

bool readRegister40(Registers reg, uint64_t* value)
{
	*value = 0;
	if (!i2cTx(INA228_ADDRESS, (uint8_t*)&reg, 1, value, 5))
		return false;

	*value = __builtin_bswap64(*value) >> 24;

	return true;
}

bool readRegister40s(Registers reg, int64_t* value)
{
	*value = 0;
	if (!i2cTx(INA228_ADDRESS, (uint8_t*)&reg, 1, value, 5))
		return false;

	*value = (int64_t)__builtin_bswap64(*value) >> 24;

	return true;
}

bool writeConfig(const Configuration& config)
{
	const uint16_t val = *(uint16_t*)&config;
	return writeRegister16(Registers::CONFIG, val);
}

bool writeAdcConfig(const AdcConfiguration& config)
{
	const uint16_t val = *(uint16_t*)&config;
	return writeRegister16(Registers::ADC_CONFIG, val);
}

bool clearStatus()
{
	int16_t i16;
	return readRegister16(Registers::DIAG_ALERT, &i16);
}

bool readDieTempValue(int16_t& val)
{
	const bool res = readRegister16(Registers::DIETEMP, &val);
	return res;
}

bool readVbusValue(uint32_t& val)
{
	const bool res = readRegister24(Registers::VBUS, (int32_t*)&val);
	val >>= 4;
	return res;
}

bool readVshuntValue(int32_t& val)
{
	const bool res = readRegister24(Registers::VSHUNT, &val);
	val >>= 4;
	return res;
}

bool readCurrentValue(int32_t& val)
{
	const bool res = readRegister24(Registers::CURRENT, &val);
	val >>= 4;
	return res;
}

bool readPowerValue(int32_t& val)
{
	return readRegister24(Registers::POWER, &val);
}

bool readEnergyValue(int64_t& val)
{
	return readRegister40s(Registers::ENERGY, &val);
}

bool readChargeValue(int64_t& val)
{
	return readRegister40s(Registers::CHARGE, &val);
}
}
