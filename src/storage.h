#pragma once

#include <avr/eeprom.h>

#include <EEPROMByteSentinelWearLevelingAsync.h>

const int EEPROMStorageRecordsCount = 13;

#pragma pack(1)
struct EEPROMReading
{
	uint8_t total_forwardCount_mAh[3];
	uint8_t total_forwardCount_mWh[3];
	uint8_t total_backwardCount_mAh[3];
	uint8_t total_backwardCount_mWh[3];
	uint8_t high_forwardCount_mAh[3];
	uint8_t high_forwardCount_mWh[3];
	uint8_t high_backwardCount_mAh[3];
	uint8_t high_backwardCount_mWh[3];
	uint8_t low_forwardCount_mAh[3];
	uint8_t low_forwardCount_mWh[3];
	uint8_t low_backwardCount_mAh[3];
	uint8_t low_backwardCount_mWh[3];
};
#pragma pack()

extern EEMEM uint8_t eeReadings[(sizeof(EEPROMReading) + 1) * EEPROMStorageRecordsCount];

extern EEPROMByteSentinelWearLevelingAsync<EEPROMReading, sizeof(eeReadings), eeReadings> eeprom;

void loadFromEEPROM();
void saveToEEPROM();