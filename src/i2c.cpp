#include "i2c.h"

bool waitForI2CInterrupt()
{
	while ((TWI0.MSTATUS & (TWI_RIF_bm | TWI_WIF_bm)) == 0) {
	}
	return true;
}

bool waitForWriteConfirm()
{
	waitForI2CInterrupt();

	if (TWI0.MSTATUS & (TWI_RXACK_bm | TWI_ARBLOST_bm | TWI_BUSERR_bm)) {
		// errors happened
		return false;
	}

	return true;
}

bool waitForReadConfirm()
{
	waitForI2CInterrupt();

	uint8_t status = TWI0.MSTATUS;
	if (status & (TWI_RXACK_bm | TWI_ARBLOST_bm | TWI_BUSERR_bm)) {
		// errors happened
		return false;
	}

	return true;
}

bool i2cTxInner(uint8_t address, const uint8_t* writeBuffer, uint8_t writeCount, uint8_t* readBuffer, uint8_t readCount)
{
	const uint8_t READ = 0x01;
	const uint8_t WRITE = 0x00;

	if (writeCount > 0) {
		TWI0.MADDR = (address << 1) | WRITE;

		if (!waitForWriteConfirm())
			return false;

		for (uint8_t i = 0; i < writeCount; i++) {
			TWI0.MDATA = writeBuffer[i];

			if (!waitForWriteConfirm())
				return false;
		}
	}

	if (readCount > 0) {
		TWI0.MADDR = (address << 1) | READ;

		if (!waitForReadConfirm())
			return false;

		TWI0.MSTATUS = TWI_WIF_bm;

		uint8_t* ptr = readBuffer;
		for (uint8_t left = readCount; left > 0; left--) {
			if (!waitForI2CInterrupt())
				return false;

			*ptr++ = TWI0.MDATA;

			if (left > 1) {
				TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc | 0;
			}
			else {
				TWI0.MCTRLB = TWI_MCMD_STOP_gc | TWI_ACKACT_bm;
			}
		}
	}
	else {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
	}

	return true;
}

bool i2cTx(uint8_t address, const void* writeBuffer, uint8_t writeCount, void* readBuffer, uint8_t readCount)
{
	if (!i2cTxInner(address, (uint8_t*)writeBuffer, writeCount, (uint8_t*)readBuffer, readCount)) {
		TWI0.MCTRLB = TWI_FLUSH_bm;
		return false;
	}
	return true;
}
