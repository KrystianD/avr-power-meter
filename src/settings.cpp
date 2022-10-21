#include "state.h"

#include <avr/eeprom.h>

EEMEM TSettings eeSetting;

TSettings readSettings()
{
	TSettings settings;
	eeprom_read_block(&settings, &eeSetting, sizeof(TSettings));
	return settings;
}
void updateSettings(TSettings& settings)
{
	eeprom_update_block(&settings, &eeSetting, sizeof(TSettings));
}