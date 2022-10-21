#include "storage.h"

#include "state.h"

EEMEM uint8_t eeReadings[(sizeof(EEPROMReading) + 1) * EEPROMStorageRecordsCount];

EEPROMByteSentinelWearLevelingAsync<EEPROMReading, sizeof(eeReadings), eeReadings> eeprom;

static void loadSummatorCounter(LSBSummator& ct, const uint8_t* src)
{
	uint8_t* dst = (uint8_t*)ct.counterRef();
	dst[0] = 0;
	dst[1] = src[0];
	dst[2] = src[1];
	dst[3] = src[2];
}

static void saveSummatorCounter(uint8_t* dst, LSBSummator& ct)
{
	uint8_t* pValue = (uint8_t*)ct.counterRef();
	dst[0] = pValue[1];
	dst[1] = pValue[2];
	dst[2] = pValue[3];
}

void loadFromEEPROM()
{
	EEPROMReading reading;

	if (eeprom.load(reading)) {
		loadSummatorCounter(global::inputTotal.trackerBackward.summator_mAh, reading.total_backwardCount_mAh);
		loadSummatorCounter(global::inputTotal.trackerBackward.summator_mWh, reading.total_backwardCount_mWh);
		loadSummatorCounter(global::inputTotal.trackerForward.summator_mAh, reading.total_forwardCount_mAh);
		loadSummatorCounter(global::inputTotal.trackerForward.summator_mWh, reading.total_forwardCount_mWh);

#ifdef HAS_LOW_SIDE
		loadSummatorCounter(global::inputHigh.trackerBackward.summator_mAh, reading.high_backwardCount_mAh);
		loadSummatorCounter(global::inputHigh.trackerBackward.summator_mWh, reading.high_backwardCount_mWh);
		loadSummatorCounter(global::inputHigh.trackerForward.summator_mAh, reading.high_forwardCount_mAh);
		loadSummatorCounter(global::inputHigh.trackerForward.summator_mWh, reading.high_forwardCount_mWh);

		loadSummatorCounter(global::inputLow.trackerBackward.summator_mAh, reading.low_backwardCount_mAh);
		loadSummatorCounter(global::inputLow.trackerBackward.summator_mWh, reading.low_backwardCount_mWh);
		loadSummatorCounter(global::inputLow.trackerForward.summator_mAh, reading.low_forwardCount_mAh);
		loadSummatorCounter(global::inputLow.trackerForward.summator_mWh, reading.low_forwardCount_mWh);
#endif
	}
}

void saveToEEPROM()
{
	EEPROMReading reading;

	saveSummatorCounter(reading.total_backwardCount_mAh, global::inputTotal.trackerBackward.summator_mAh);
	saveSummatorCounter(reading.total_backwardCount_mWh, global::inputTotal.trackerBackward.summator_mWh);
	saveSummatorCounter(reading.total_forwardCount_mAh, global::inputTotal.trackerForward.summator_mAh);
	saveSummatorCounter(reading.total_forwardCount_mWh, global::inputTotal.trackerForward.summator_mWh);

#ifdef HAS_LOW_SIDE
	saveSummatorCounter(reading.high_backwardCount_mAh, global::inputHigh.trackerBackward.summator_mAh);
	saveSummatorCounter(reading.high_backwardCount_mWh, global::inputHigh.trackerBackward.summator_mWh);
	saveSummatorCounter(reading.high_forwardCount_mAh, global::inputHigh.trackerForward.summator_mAh);
	saveSummatorCounter(reading.high_forwardCount_mWh, global::inputHigh.trackerForward.summator_mWh);

	saveSummatorCounter(reading.low_backwardCount_mAh, global::inputLow.trackerBackward.summator_mAh);
	saveSummatorCounter(reading.low_backwardCount_mWh, global::inputLow.trackerBackward.summator_mWh);
	saveSummatorCounter(reading.low_forwardCount_mAh, global::inputLow.trackerForward.summator_mAh);
	saveSummatorCounter(reading.low_forwardCount_mWh, global::inputLow.trackerForward.summator_mWh);
#endif

	eeprom.save(reading);
}
