#include "state.h"

#include "config.h"

namespace global
{
TConfig* currentConfiguration;

MovingAverage<6> shunt_mA;
InputData inputTotal;

#ifdef HAS_LOW_SIDE
InputData inputHigh;
InputData inputLow;
#endif
}
