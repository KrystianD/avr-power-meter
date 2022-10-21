#include <stdint.h>

enum class INA226Averages : uint8_t
{
	Average1 = 0,
	Average4 = 1,
	Average16 = 2,
	Average64 = 3,
	Average128 = 4,
	Average256 = 5,
	Average512 = 6,
	Average1024 = 7,
};

enum class INA226ConversionTime : uint8_t
{
	Conv140us = 0,
	Conv204us = 1,
	Conv332us = 2,
	Conv588us = 3,
	Conv1100us = 4,
	Conv2116us = 5,
	Conv4156us = 6,
	Conv8244us = 7,
};

struct INA226Configuration
{
	INA226Averages avg: 3;
	INA226ConversionTime vbusctl: 3;
	INA226ConversionTime vshct: 3;
};
