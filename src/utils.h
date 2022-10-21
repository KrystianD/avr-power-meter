#pragma once

#include <stdint.h>

static int16_t swapS16(int16_t val)
{
	return (val << 8) | ((val >> 8) & 0xFF);
}