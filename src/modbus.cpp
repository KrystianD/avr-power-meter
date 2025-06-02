#include <ksystem.h>

#include "global.h"

void reboot()
{
	_PROTECTED_WRITE(RSTCTRL.SWRR, 1);
}

#ifdef MODBUS_ENABLED
bool modbusHandleReadInputRegister(uint16_t address, ModbusReturnValue& value)
{
	switch (address) {
		case 0:
			return value.FLOATBE(global::state.vbus);
		case 2:
			return value.FLOATBE(global::state.current);
		case 4:
			return value.FLOATBE(global::state.power);
		case 6:
			return value.FLOATBE(global::state.charge);
		case 8:
			return value.FLOATBE(global::state.energy);
		case 10:
			return value.FLOATBE(global::state.dieTemp);

		case 12:
			return value.S16(global::state.raw_dieTemp);
		case 14:
			return value.U32BE(global::state.raw_vbus);
		case 16:
			return value.S32BE(global::state.raw_vshunt);
		case 18:
			return value.S32BE(global::state.raw_current);
		case 20:
			return value.S32BE(global::state.raw_power);
		case 22:
			return value.U32BE(global::state.raw_energy >> 32);
		case 24:
			return value.U32BE(global::state.raw_energy);
		case 26:
			return value.U32BE(global::state.raw_charge >> 32);
		case 28:
			return value.U32BE(global::state.raw_charge);

		default:
			return false;
	}
}

bool modbusHandleReadHoldingRegister(uint16_t address, ModbusReturnValue& value)
{
	Config config = global::eeConfig.get();

	switch (address) {
		case 0:
			return value.U16(config.slaveId);
		case 1:
			return value.U16(config.baudRate);
		case 2:
			return value.U16(config.shuntRatedCurrent_A);
		case 3:
			return value.U16(config.shuntRatedVoltageDrop_mV);
		case 4:
			return value.U16((int)config.averages);
		case 5:
			return value.U16((int)config.conversionTime);
		default:
			return false;
	}
}

#define CHECK_RANGE(x, min, max) do { if (x < min || x > max) return false; } while(0)

bool modbusHandleWriteHoldingRegister(uint16_t address, ModbusValue value)
{
	Config config = global::eeConfig.get();

	switch (address) {
		case 0:
			CHECK_RANGE(value.u16, 1, 254);
			config.slaveId = value.u16;
			global::eeConfig.set(config);
			return true;
		case 1:
			CHECK_RANGE(value.u16, 1, 1);
			config.baudRate = value.u16;
			global::eeConfig.set(config);
			return true;
		case 2:
			CHECK_RANGE(value.u16, 1, 1000);
			config.shuntRatedCurrent_A = value.u16;
			global::eeConfig.set(config);
			return true;
		case 3:
			CHECK_RANGE(value.u16, 1, 1000);
			config.shuntRatedVoltageDrop_mV = value.u16;
			global::eeConfig.set(config);
			return true;
		case 4:
			CHECK_RANGE(value.u16, (int)ina228::Averages::Min, (int)ina228::Averages::Max);
			config.averages = (ina228::Averages)value.u16;
			global::eeConfig.set(config);
			return true;
		case 5:
			CHECK_RANGE(value.u16, (int)ina228::ConversionTime::Min, (int)ina228::ConversionTime::Max);
			config.conversionTime = (ina228::ConversionTime)value.u16;
			global::eeConfig.set(config);
			return true;
		case 100:
			if (value.u16 == 1)
				reboot();
			return true;
		default:
			return false;
	}
	return false;
}
#endif
