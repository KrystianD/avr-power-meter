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
#include "storage.h"
#include "debug.h"

const uint8_t INA226_HIGH_ADDR = 0x40;
#ifdef HAS_LOW_SIDE
const uint8_t INA226_LOW_ADDR = 0x42;
#endif

void performMeasurement();
void processInputValues(InputData& side, int16_t shuntLSB, uint16_t busLSB);

uint16_t saveCounter = 0;
bool ina226Initialized = false;

bool initializeIna226()
{
#ifdef HAS_I2C
	// reset INA226
	if (!i2cINA226WriteRegister(INA226_HIGH_ADDR, 0, 0x8000)) return false;
#ifdef HAS_LOW_SIDE
	if (!i2cINA226WriteRegister(INA226_LOW_ADDR, 0, 0x8000)) return false;
#endif

	// configure INA226
	const uint16_t ina226_modeBits = 0b111u;
	uint16_t configReg =
			((uint8_t)INA226Averages::Average16 << 9u) |
			((uint8_t)INA226ConversionTime::Conv140us << 6u) |
			((uint8_t)INA226ConversionTime::Conv2116us << 3u) |
			(ina226_modeBits << 0);
	if (!i2cINA226WriteRegister(INA226_HIGH_ADDR, 0, configReg)) return false;
#ifdef HAS_LOW_SIDE
	if (!i2cINA226WriteRegister(INA226_LOW_ADDR, 0, configReg)) return false;
#endif
#endif

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

	// read slave id from address pins
	addr1.inputPullup();
	addr2.inputPullup();
	addr4.inputPullup();

	_delay_ms(10);
	uint8_t pinAddr = (addr1.isLow() << 0) | (addr2.isLow() << 1) | (addr4.isLow() << 2);
	uint8_t slaveId = pinAddr != 0 ? pinAddr : settings.slaveId;

	addr1.low();
	addr2.low();
	addr4.low();

	global::currentConfiguration = &configs[settings.configNum];

	global::inputTotal.trackerForward.setConfig(global::currentConfiguration);
	global::inputTotal.trackerBackward.setConfig(global::currentConfiguration);
#ifdef HAS_LOW_SIDE
	global::inputHigh.trackerForward.setConfig(global::currentConfiguration);
	global::inputHigh.trackerBackward.setConfig(global::currentConfiguration);
	global::inputLow.trackerForward.setConfig(global::currentConfiguration);
	global::inputLow.trackerBackward.setConfig(global::currentConfiguration);
#endif

	loadFromEEPROM();

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
	if (!i2cReadINA226(INA226_HIGH_ADDR, shuntLSB, total_busLSB)) {
		ina226Initialized = false;
		return;
	}
#ifdef HAS_LOW_SIDE
	int16_t dummy;
	uint16_t low_busLSB;
	if (!i2cReadINA226(INA226_LOW_ADDR, dummy, low_busLSB)) {
		ina226Initialized = false;
		return;
	}
	uint16_t high_busLSB = total_busLSB - low_busLSB;
#endif

	shuntLSB += 2;
	int32_t shunt_mA = SHUNTLSB_TO_MILLIAMPERE(shuntLSB, global::currentConfiguration);
	global::shunt_mA.add(shunt_mA);

	processInputValues(global::inputTotal, shuntLSB, total_busLSB);

#ifdef HAS_LOW_SIDE
	if (high_busLSB > MILLIVOLT_TO_BUSLSB(2000) && low_busLSB > MILLIVOLT_TO_BUSLSB(2000)) {
		processInputValues(global::inputHigh, shuntLSB, high_busLSB);
		processInputValues(global::inputLow, shuntLSB, low_busLSB);
	}
#endif

	saveCounter++;

	if (saveCounter == SaveIntervalSeconds * (1000 / READ_INTERVAL_MILLISECONDS)) {
		saveCounter = 0;
		led.toggle();
		saveToEEPROM();
	}

	if (MODBUS_ENABLED && METER_DEBUG) {
		METER_PRINTF("mV %6ld mA %6ld mA %6ld mW, fw %6ld, bw %6ld\r\n",
		             global::inputTotal.bus_mV.getValue(),
		             global::shunt_mA.getValue(),
		             global::inputTotal.power_mW.getValue(),
		             global::inputTotal.trackerForward.summator_mWh.getCount(),
		             global::inputTotal.trackerBackward.summator_mWh.getCount());

		global::inputTotal.trackerForward.print();
		global::inputTotal.trackerBackward.print();
	}
#endif
}

void processInputValues(InputData& side, int16_t shuntLSB, uint16_t busLSB)
{
	int32_t bus_mV = BUSLSB_TO_MILLIVOLT(busLSB);
	int32_t power_mW = SHUNTBUSLSB_TO_MILLIWATT(shuntLSB, busLSB, global::currentConfiguration);

	side.bus_mV.add(bus_mV);
	side.power_mW.add(power_mW);

	if (shuntLSB < 0)
		side.trackerBackward.addLSB(-shuntLSB, busLSB);
	else
		side.trackerForward.addLSB(shuntLSB, busLSB);
}

#pragma clang diagnostic pop
