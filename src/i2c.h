#pragma once

#include <stdint.h>

void i2cInit();
bool i2cTx(uint8_t addr, void* tx, uint8_t txCount, void* rx, uint8_t rxCount);
bool i2cINA226ReadRegister(uint8_t addr, uint8_t reg, uint16_t& value);
bool i2cINA226WriteRegister(uint8_t addr, uint8_t reg, uint16_t value);
bool i2cReadINA226(uint8_t addr, int16_t& shuntLSB, uint16_t& busLSB);