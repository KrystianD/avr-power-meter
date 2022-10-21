#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-deprecated-headers"
#pragma ide diagnostic ignored "readability-static-accessed-through-instance"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <stdint.h>

#include <util/delay.h>

#include <ksystem.h>
#include <configurations.h>

#include "config.h"
#include "ina226.h"
#include "i2c.h"
#include "state.h"
#include "debug.h"

const uint8_t INA226_ADDR = 0x40;

void performMeasurement();
void processInputValues(int16_t shuntLSB, uint16_t busLSB);

bool ina226Initialized = false;

bool initializeIna226()
{
	// reset INA226
	if (!i2cINA226WriteRegister(INA226_ADDR, 0, 0x8000)) return false;

	// configure INA226
	const uint16_t ina226_modeBits = 0b111u;
	uint16_t configReg =
			((uint8_t)INA226Averages::Average16 << 9u) |
			((uint8_t)INA226ConversionTime::Conv140us << 6u) |
			((uint8_t)INA226ConversionTime::Conv2116us << 3u) |
			(ina226_modeBits << 0);
	if (!i2cINA226WriteRegister(INA226_ADDR, 0, configReg)) return false;

	return true;
}

void setup()
{
#ifdef HAS_I2C
	i2cInit();
#endif

	// read configuration from EEPROM
	TSettings settings = readSettings();

	if (settings.configNum == 0xff || settings.slaveId == 0) {
		settings.configNum = 0;
		settings.slaveId = 1;
		updateSettings(settings);
	}

	uint8_t slaveId = settings.slaveId;

	global::currentConfiguration = &configs[settings.configNum];

	global::inputTotal.trackerForward.setConfig(global::currentConfiguration);
	global::inputTotal.trackerBackward.setConfig(global::currentConfiguration);

	for (int i = slaveId * 2; i > 0; i--) {
		led.toggle();
		_delay_ms(200);
	}

	readTimer.start(READ_INTERVAL_MILLISECONDS);

#ifdef MODBUS_ENABLED
	Modbus.start(slaveId);
#else
	dir.high();
#endif
}

void loop()
{
	if (readTimer.elapsed()) {
		if (!ina226Initialized) {
			if (initializeIna226())
				ina226Initialized = true;
		}

		if (ina226Initialized) {
			performMeasurement();
		}
	}
}

void performMeasurement()
{
#ifdef HAS_I2C
	int16_t shuntLSB;
	uint16_t total_busLSB;
	if (!i2cReadINA226(INA226_ADDR, shuntLSB, total_busLSB)) {
		ina226Initialized = false;
		return;
	}

	shuntLSB += 2;
	int32_t shunt_mA = SHUNTLSB_TO_MILLIAMPERE(shuntLSB, global::currentConfiguration);
	global::shunt_mA.add(shunt_mA);

	processInputValues(shuntLSB, total_busLSB);

	if (MODBUS_ENABLED && METER_DEBUG) {
		METER_PRINTF("mV %6ld mA %6ld mA %6ld mW, fw %6ld, bw %6ld\r\n",
		             global::inputTotal.bus_mV.getValue(),
		             global::shunt_mA.getValue(),
		             global::inputTotal.power_mW.getValue(),
		             global::inputTotal.trackerForward.summator_mWh.getCount(),
		             global::inputTotal.trackerBackward.summator_mWh.getCount());
	}
#endif
}

void processInputValues(int16_t shuntLSB, uint16_t busLSB)
{
	int32_t bus_mV = BUSLSB_TO_MILLIVOLT(busLSB);
	int32_t power_mW = SHUNTBUSLSB_TO_MILLIWATT(shuntLSB, busLSB, global::currentConfiguration);

	global::inputTotal.bus_mV.add(bus_mV);
	global::inputTotal.power_mW.add(power_mW);

	if (shuntLSB < 0)
		global::inputTotal.trackerBackward.addLSB(-shuntLSB, busLSB);
	else
		global::inputTotal.trackerForward.addLSB(shuntLSB, busLSB);
}

#pragma clang diagnostic pop
