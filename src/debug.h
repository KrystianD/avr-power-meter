#pragma once

#include "config.h"

#ifdef METER_DEBUG
#include <stdio.h>
#define METER_PRINTF printf
#else
#define METER_PRINTF(x, ...)
#define METER_DEBUG 0
#endif