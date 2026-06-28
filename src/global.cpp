#include "global.h"

namespace global
{
GlobalState state;

EEMEM EEPROMMemory<Config> eeConfig;

int requestRebootCounter = 0;
}
