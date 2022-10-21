#include <ksystem.h>

#include "state.h"

#ifdef MODBUS_ENABLED
bool modbusHandleReadInputRegister(uint16_t address, ModbusReturnValue& value)
{
	switch (address) {
		case 1:
			return value.U16(global::inputTotal.bus_mV.getValue() / 10);
#ifdef HAS_LOW_SIDE
		case 2:
			return value.U16(global::inputHigh.bus_mV.getValue() / 10);
		case 3:
			return value.U16(global::inputLow.bus_mV.getValue() / 10);
#endif
		case 4:
			return value.S16(global::shunt_mA.getValue() / 10);
		case 5:
			return value.S16(global::inputTotal.power_mW.getValue() / 1000);
#ifdef HAS_LOW_SIDE
		case 6:
			return value.S16(global::inputHigh.power_mW.getValue() / 1000);
		case 7:
			return value.S16(global::inputLow.power_mW.getValue() / 1000);
#endif

		case 8:
			return value.U32LE(global::inputTotal.trackerForward.summator_mAh.getCount() / 10);
		case 10:
			return value.U32LE(global::inputTotal.trackerForward.summator_mWh.getCount() / 10);

		case 12:
			return value.U32LE(global::inputTotal.trackerBackward.summator_mAh.getCount() / 10);
		case 14:
			return value.U32LE(global::inputTotal.trackerBackward.summator_mWh.getCount() / 10);

#ifdef HAS_LOW_SIDE
		case 16:
			return value.U32LE(global::inputHigh.trackerForward.summator_mAh.getCount() / 10);
		case 18:
			return value.U32LE(global::inputHigh.trackerForward.summator_mWh.getCount() / 10);

		case 20:
			return value.U32LE(global::inputHigh.trackerBackward.summator_mAh.getCount() / 10);
		case 22:
			return value.U32LE(global::inputHigh.trackerBackward.summator_mWh.getCount() / 10);

		case 24:
			return value.U32LE(global::inputLow.trackerForward.summator_mAh.getCount() / 10);
		case 26:
			return value.U32LE(global::inputLow.trackerForward.summator_mWh.getCount() / 10);

		case 28:
			return value.U32LE(global::inputLow.trackerBackward.summator_mAh.getCount() / 10);
		case 30:
			return value.U32LE(global::inputLow.trackerBackward.summator_mWh.getCount() / 10);
#endif

		default:
			return false;
	}
}

bool modbusHandleReadHoldingRegister(uint16_t address, ModbusReturnValue& value)
{
	TSettings settings = readSettings();

	switch (address) {
		case 1:
			return value.U16(settings.configNum);
		case 2:
			return value.U16(settings.slaveId);
		default:
			return false;
	}
}

#define CHECK_RANGE(x, min, max) do { if (x < min || x > max) return false; } while(0)

bool modbusHandleWriteHoldingRegister(uint16_t address, ModbusValue value)
{
	TSettings settings = readSettings();

	switch (address) {
		case 1:
			CHECK_RANGE(value.u16, 0, ConfigurationsCount - 1);
			settings.configNum = value.u16;
			updateSettings(settings);
			return true;
		case 2:
			CHECK_RANGE(value.u16, 1, 254);
			settings.slaveId = value.u16;
			updateSettings(settings);
			return true;
		default:
			return false;
	}
}
#endif