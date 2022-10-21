#pragma once

#include <stdint.h>

#pragma pack(1)
struct TSettings
{
	uint8_t configNum, slaveId;
};
#pragma pack()

TSettings readSettings();
void updateSettings(TSettings& settings);