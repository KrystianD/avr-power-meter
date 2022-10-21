#include "i2c.h"

#include <avr/io.h>

#include "utils.h"

#define TWBR_VAL  ((F_CPU / 100000UL - 16) / 2)


uint8_t TWI_Start();
uint8_t TWI_WriteByte(uint8_t value);
uint8_t TWI_ReadByte(uint8_t* value, bool ack);
uint8_t TWI_Stop();

void i2cInit()
{
	TWBR = TWBR_VAL;
}

bool i2cTx(uint8_t addr, void* tx, uint8_t txCount, void* rx, uint8_t rxCount)
{
	if (txCount > 0) {
		TWI_Start();
		TWI_WriteByte((uint8_t)(addr << 1u) | 0u);
		for (uint8_t i = 0; i < txCount; i++)
			TWI_WriteByte(((uint8_t*)tx)[i]);
	}

	if (rxCount > 0) {
		TWI_Start();
		TWI_WriteByte((uint8_t)(addr << 1u) | 1u);
		for (uint8_t i = 0; i < rxCount; i++)
			TWI_ReadByte(&((uint8_t*)rx)[i], i != rxCount - 1);
	}

	TWI_Stop();
	return true;
}

bool i2cINA226ReadRegister(uint8_t addr, uint8_t reg, uint16_t& value)
{
	if (!i2cTx(addr, &reg, 1, (uint8_t*)&value, 2))
		return false;

	value = swapS16(value);

	return true;
}

bool i2cINA226WriteRegister(uint8_t addr, uint8_t reg, uint16_t value)
{
	uint8_t tx[] = { reg,
	                 (uint8_t)((uint8_t)(value >> 8u) & 0xffu),
	                 (uint8_t)((uint8_t)(value >> 0u) & 0xffu),
	};
	return i2cTx(addr, &tx, 3, nullptr, 0);
}

bool i2cReadINA226(uint8_t addr, int16_t& shuntLSB, uint16_t& busLSB)
{
	if (!i2cINA226ReadRegister(addr, 1, *(uint16_t*)&shuntLSB)) return false;
	if (!i2cINA226ReadRegister(addr, 2, busLSB)) return false;
	return true;
}

uint8_t TWI_Start()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);

	while (bit_is_clear(TWCR, TWINT));

	return 0;
}
uint8_t TWI_WriteByte(uint8_t value)
{
	TWDR = value;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while (bit_is_clear(TWCR, TWINT));

	return 0;
}
uint8_t TWI_ReadByte(uint8_t* value, bool ack)
{
	TWCR = (1 << TWINT) | (ack << TWEA) | (1 << TWEN);

	while (bit_is_clear(TWCR, TWINT));
	*value = TWDR;

	if (ack) {
		return 0;
	}
	else {
		return 0;
	}

	return 0;
}
uint8_t TWI_Stop()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while (bit_is_set(TWCR, TWSTO));

	return 0;
}