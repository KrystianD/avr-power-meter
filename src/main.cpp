#include <stdint.h>

#include <ksystem.h>

#include "ina228.h"
#include "global.h"

float currentLsb;

void setup()
{
	scl.input();
	sda.input();
	led.low();

	Config config = global::eeConfig.get();

#define TWI_BAUD(freq, t_rise) (((F_CPU / freq) / 2) - (5 + (((F_CPU / 1000000) * t_rise) / 2000)))

	TWI0.MBAUD = TWI_BAUD(100000, 600);
	TWI0.MCTRLA = TWI_ENABLE_bm;
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;

	ina228::Configuration cfg{};
	cfg.reset = true;
	ina228::writeConfig(cfg);

	ina228::AdcConfiguration conf = {
		.avg = config.averages,
		.vtempct = ina228::ConversionTime::Conv50us,
		.vshuntct = config.conversionTime,
		.vbusct = config.conversionTime,
		.mode = ina228::Mode::ContinuousTemperatureBusShunt,
	};

	ina228::writeAdcConfig(conf);
	ina228::writeRegister16(ina228::Registers::DIAG_ALERT, _BV(14));

	currentLsb = (float)config.shuntRatedCurrent_A / 524288.0f; // 2^19
	uint16_t shuntCal = (uint16_t)(100000.0f * (float)config.shuntRatedVoltageDrop_mV / 1000.0f / 4.0);

	ina228::writeRegister16(ina228::Registers::SHUNT_CAL, shuntCal);

	Modbus.start(config.slaveId);
}

void loop()
{
	static int i = 0;
	if (alert.isLow()) {
		ina228::clearStatus();

		ina228::readDieTempValue(global::state.raw_dieTemp);
		ina228::readVbusValue(global::state.raw_vbus);
		ina228::readVshuntValue(global::state.raw_vshunt);
		ina228::readCurrentValue(global::state.raw_current);
		ina228::readPowerValue(global::state.raw_power);
		ina228::readEnergyValue(global::state.raw_energy);
		ina228::readChargeValue(global::state.raw_charge);

		constexpr float Scale_mV = 1000000.0f;
		constexpr float Scale_nV = 1000000000.0f;

		global::state.dieTemp = (float)global::state.raw_dieTemp * (7.8125f / 1000.0f);
		global::state.vbus = (float)global::state.raw_vbus * (195.3125f / Scale_mV);
		global::state.vshunt = (float)global::state.raw_vshunt * (312.5f / Scale_nV);
		global::state.current = currentLsb * (float)global::state.raw_current;
		global::state.power = currentLsb * (float)global::state.raw_power * 3.2f /* [W] */;
		global::state.energy = currentLsb * (float)global::state.raw_energy * (3.2f * 16) /* [Ws] */ / 3600.0f /* [Wh] */;
		global::state.charge = currentLsb * (float)global::state.raw_charge /* [As] */ / 3600.0f /* [Ah] */;

		i++;
		if (i == 100) {
			printf("----\r\n");
			printf("DIETEMP = %10d, %10.2f\r\n", global::state.raw_dieTemp, global::state.dieTemp);
			printf("VBUS    = %10ld, %10.2f\r\n", global::state.raw_vbus, global::state.vbus);
			printf("VSHUNT  = %10ld, %10.2f\r\n", global::state.raw_vshunt, global::state.vshunt);
			printf("CURRENT = %10ld, %10.2f\r\n", global::state.raw_current, global::state.current);
			printf("POWER   = %10ld, %10.2f\r\n", global::state.raw_power, global::state.power);
			printf("ENERGY  = %10.0f, %10f\r\n", (float)global::state.raw_energy, global::state.energy);
			printf("CHARGE  = %10.0f, %10f\r\n", (float)global::state.raw_charge, global::state.charge);
			i = 0;
		}
	}
}
