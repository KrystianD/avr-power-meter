#pragma once

template<int Ratio>
class MovingAverage
{
	int32_t value = 0;

public:
	void add(int32_t val)
	{
		value = (val * Ratio + value * (100 - Ratio)) / 100;
	}

	int32_t getValue() const { return value; }
};
