#pragma once

#include <stdio.h>

#include <configurations.h>

#include "debug.h"

class LSBSummator
{
	uint32_t _lsbPerCount = 0;

	uint32_t _lsbSum = 0;
	uint32_t _count = 0;

public:
	void setLSBPerCount(uint32_t lsbPerCount)
	{
		_lsbPerCount = lsbPerCount;
	}

	void addLSB(uint32_t lsbValue)
	{
		_lsbSum += lsbValue;

		while (_lsbSum > _lsbPerCount) {
			_lsbSum -= _lsbPerCount;
			_count++;
		}
	}

	uint32_t* counterRef() { return &_count; }
	[[nodiscard]] uint32_t getCount() const { return _count; }
	[[nodiscard]] uint32_t getCurrentLSB() const { return _lsbSum; }
};

class CurrentTracker
{
public:
	LSBSummator summator_mAh;
	LSBSummator summator_mWh;

	void setConfig(const TConfig* cfg)
	{
		summator_mAh.setLSBPerCount(cfg->shuntlsbsum_per_milliamperehour);
		summator_mWh.setLSBPerCount(cfg->shuntbuslsbsum_per_milliwatthour);
	}

	void addLSB(uint16_t shuntLSB, uint16_t busLSB)
	{
		summator_mAh.addLSB(shuntLSB);
		summator_mWh.addLSB((uint32_t)shuntLSB * (uint32_t)busLSB);
	}

	void print() const
	{
		METER_PRINTF("mAh %7lu (%8lu)\r\n", summator_mAh.getCount(), summator_mAh.getCurrentLSB());
		METER_PRINTF("mWh %7lu (%8lu)\r\n", summator_mWh.getCount(), summator_mWh.getCurrentLSB());
	}
};