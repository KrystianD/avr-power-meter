#pragma once

#include <ksystem.h>

bool i2cTx(uint8_t address, const void* writeBuffer, uint8_t writeCount, void* readBuffer, uint8_t readCount);