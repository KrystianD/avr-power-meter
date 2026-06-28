
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "ksystem.h"
#include "ksystem_internal.h"

ISR(RTC_CNT_vect)
{
  SysTick.handleInterrupt();
}
ISR(USART1_RXC_vect)
{
  pxMBFrameCBByteReceived();
}
ISR(USART1_DRE_vect)
{
  pxMBFrameCBTransmitterEmpty();
}
kConfiguredSysTick SysTick;
extern void setup();
extern void loop();

// Component: InterruptsComponent

// Component: SerialComponent

// Component: SerialComponent

// Component: SysTickComponent

// Component: DummyStdioComponent

// Component: GPIOComponent

// Component: ModbusComponent

int main()
{
  _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, 0);
  _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0);
  // Component: InterruptsComponent
  {
  }

  // Component: SerialComponent
  {
    kGPIO<kPORTB,2> pinTx;
    pinTx.pushPull();
    USART0.BAUD = 1111;
    USART0.CTRLA = 0;
    USART0.CTRLB = USART_TXEN_bm | USART_RXEN_bm | USART_RXMODE_CLK2X_gc;
  }

  // Component: SerialComponent
  {
    kGPIO<kPORTA,1> pinTx;
    pinTx.pushPull();
    kGPIO<kPORTA,4> pinXDir;
    pinXDir.pushPull();
    pinXDir.low();
    USART1.BAUD = 1111;
    USART1.CTRLA = USART_RS485_bm;
    USART1.CTRLB = USART_TXEN_bm | USART_RXEN_bm | USART_RXMODE_CLK2X_gc;
  }

  // Component: SysTickComponent
  {
    SysTick.init();
  }

  // Component: DummyStdioComponent
  {
  }

  // Component: GPIOComponent
  {
    led.pushPull();
  }

  // Component: ModbusComponent
  {
    Modbus.init();
  }

  setup();
  sei();
  for (; ; )
  {
    loop();

    // Component: InterruptsComponent

    // Component: SerialComponent

    // Component: SerialComponent

    // Component: SysTickComponent

    // Component: DummyStdioComponent

    // Component: GPIOComponent

    // Component: ModbusComponent
    Modbus.process();
  }
}