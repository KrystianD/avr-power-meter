from datetime import timedelta
from fractions import Fraction

read_interval = timedelta(milliseconds=50)

milli = Fraction(1, 1000)
micro = Fraction(1, 1000 * 1000)

shunt_lsb_to_volt = Fraction(25, 10) * micro
bus_lsb_to_volt = Fraction(125, 100) * milli

shunt_lsb_to_millivolt: Fraction = shunt_lsb_to_volt * 1000
bus_lsb_to_millivolt: Fraction = bus_lsb_to_volt * 1000

bus_lsb_to_shunt_lsb = bus_lsb_to_volt / shunt_lsb_to_volt

f_header = open("generated/configurations.h", "wt")

read_interval_ms = int(read_interval.total_seconds() * 1000)

f_header.write(f"""
#pragma once

#include <stdint.h>

static int READ_INTERVAL_MILLISECONDS = {read_interval_ms};

static int16_t SHUNTLSB_TO_BUSLSB(int16_t x) {{ return x / {bus_lsb_to_shunt_lsb}; }}

static uint16_t MILLIVOLT_TO_BUSLSB(uint16_t x) {{ return (uint32_t)x * {bus_lsb_to_millivolt.denominator} / {bus_lsb_to_millivolt.numerator}; }}
static uint16_t BUSLSB_TO_MILLIVOLT(uint16_t x) {{ return (uint32_t)x * {bus_lsb_to_millivolt.numerator} / {bus_lsb_to_millivolt.denominator}; }}
static int16_t SHUNTLSB_TO_MILLIVOLT(int16_t x) {{ return (int32_t)x * {shunt_lsb_to_millivolt.numerator} / {shunt_lsb_to_millivolt.denominator}; }}

static struct TConfig
{{
	uint8_t shuntlsb_to_milliampere_num;
	uint8_t shuntlsb_to_milliampere_denom;
	uint32_t shuntlsbsum_per_milliamperehour;
	uint32_t shuntbuslsbsum_per_milliwatthour;
}} configs[] = {{""".lstrip())

cnt = 0


def gen_config(shunt_nominal_voltage_drop_millivolt, shunt_nominal_ampere_per_voltage_drop):
    global cnt
    cnt += 1

    shunt_nominal_voltage_drop_volt = Fraction(shunt_nominal_voltage_drop_millivolt, 1000)

    shunt_volt_to_ampere = Fraction(shunt_nominal_ampere_per_voltage_drop, shunt_nominal_voltage_drop_volt)

    read_intervals_in_one_hour = int(timedelta(hours=1) / read_interval)

    interval_to_hour = Fraction(1, read_intervals_in_one_hour)

    shuntlsb_to_ampere = shunt_lsb_to_volt * shunt_volt_to_ampere

    shuntlsb_to_amperehour = shuntlsb_to_ampere * interval_to_hour
    shuntlsb_per_amperehour = 1 / shuntlsb_to_amperehour

    buslsb_per_volt = 1 / bus_lsb_to_volt

    shuntbuslsb_per_watthour = shuntlsb_per_amperehour * buslsb_per_volt

    shuntlsb_per_milliamperehour = shuntlsb_per_amperehour / 1000
    shuntbuslsb_per_milliwatthour = shuntbuslsb_per_watthour / 1000

    shuntlsb_to_milliampere = shuntlsb_to_ampere * 1000

    f_header.write(f"""
				{{
								// {shunt_nominal_voltage_drop_millivolt}mV / {shunt_nominal_ampere_per_voltage_drop} A
								.shuntlsb_to_milliampere_num = {shuntlsb_to_milliampere.numerator},
								.shuntlsb_to_milliampere_denom = {shuntlsb_to_milliampere.denominator},
								.shuntlsbsum_per_milliamperehour = {round(shuntlsb_per_milliamperehour)}, // {shuntlsb_per_milliamperehour} = {float(shuntlsb_per_milliamperehour):.2f}
								.shuntbuslsbsum_per_milliwatthour = {round(shuntbuslsb_per_milliwatthour)}, // {shuntbuslsb_per_milliwatthour} = {float(shuntbuslsb_per_milliwatthour):.2f}
				}},""")


gen_config(75, 50)
gen_config(75, 100)

f_header.write(f"""
}};

const uint8_t ConfigurationsCount = {cnt};

static int32_t SHUNTLSB_TO_MILLIAMPERE(int16_t shuntLSB, TConfig* config)
{{
	return (int32_t)shuntLSB * (int32_t)config->shuntlsb_to_milliampere_num / (int32_t)config->shuntlsb_to_milliampere_denom;
}}
static int32_t SHUNTBUSLSB_TO_MILLIWATT(int16_t shuntLSB, uint16_t busLSB, TConfig* config)
{{
	return SHUNTLSB_TO_MILLIAMPERE(shuntLSB, config) * (int32_t)BUSLSB_TO_MILLIVOLT(busLSB) / 1000;
}}
""")
