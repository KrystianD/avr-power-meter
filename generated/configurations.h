#pragma once

#include <stdint.h>

static int READ_INTERVAL_MILLISECONDS = 50;

static int16_t SHUNTLSB_TO_BUSLSB(int16_t x) { return x / 500; }

static uint16_t MILLIVOLT_TO_BUSLSB(uint16_t x) { return (uint32_t)x * 4 / 5; }
static uint16_t BUSLSB_TO_MILLIVOLT(uint16_t x) { return (uint32_t)x * 5 / 4; }
static int16_t SHUNTLSB_TO_MILLIVOLT(int16_t x) { return (int32_t)x * 1 / 400; }

static struct TConfig
{
	uint8_t shuntlsb_to_milliampere_num;
	uint8_t shuntlsb_to_milliampere_denom;
	uint32_t shuntlsbsum_per_milliamperehour;
	uint32_t shuntbuslsbsum_per_milliwatthour;
} configs[] = {
				{
								// 75mV / 50 A
								.shuntlsb_to_milliampere_num = 5,
								.shuntlsb_to_milliampere_denom = 3,
								.shuntlsbsum_per_milliamperehour = 43200, // 43200 = 43200.00
								.shuntbuslsbsum_per_milliwatthour = 34560000, // 34560000 = 34560000.00
				},
				{
								// 75mV / 100 A
								.shuntlsb_to_milliampere_num = 10,
								.shuntlsb_to_milliampere_denom = 3,
								.shuntlsbsum_per_milliamperehour = 21600, // 21600 = 21600.00
								.shuntbuslsbsum_per_milliwatthour = 17280000, // 17280000 = 17280000.00
				},
};

const uint8_t ConfigurationsCount = 2;

static int32_t SHUNTLSB_TO_MILLIAMPERE(int16_t shuntLSB, TConfig* config)
{
	return (int32_t)shuntLSB * (int32_t)config->shuntlsb_to_milliampere_num / (int32_t)config->shuntlsb_to_milliampere_denom;
}
static int32_t SHUNTBUSLSB_TO_MILLIWATT(int16_t shuntLSB, uint16_t busLSB, TConfig* config)
{
	return SHUNTLSB_TO_MILLIAMPERE(shuntLSB, config) * (int32_t)BUSLSB_TO_MILLIVOLT(busLSB) / 1000;
}
