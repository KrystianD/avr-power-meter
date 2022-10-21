#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "ksystem.h"

template<> volatile uint8_t kConfiguredSysTick::_overflowCounter = 0;
template<> volatile uint8_t kConfiguredSysTick::_nextStep = 0;
template<> volatile uint16_t kConfiguredSysTick::_ticks = 0;

kSysTimer<1,nullptr,true> readTimer;
kSysTimer<1,modbusTimerElapsed,false> modbusTimer;
extern void setup();
extern void loop();

// Component: SerialComponent

// Component: SysTickComponent
SIGNAL(TIMER2_COMP_vect)
{
  SysTick::handleInterrupt();
}
void kSysTickElapsed()
{
  readTimer.handleTick();
  modbusTimer.handleTick();
}

// Component: GPIOComponent

// Component: ModbusComponent

int main()
{
  // Component: SerialComponent
  const uint16_t UBRR = 103;
  UBRRH = (uint8_t)(UBRR >> 8u) & 0xffu;
  UBRRL = (uint8_t)(UBRR >> 0u) & 0xffu;
  UCSRA = 0;
  UCSRB = _BV(RXEN) | _BV(TXEN);
  UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);

  // Component: SysTickComponent
  TCNT2 = 0;
  OCR2 = 255;
  TCCR2 = _BV(WGM21) | _BV(CS22) | _BV(CS20);
  TIMSK = _BV(OCIE2);
  SysTick::init();

  // Component: GPIOComponent
  led.pushPull();
  out3.pushPull();
  out2.pushPull();
  out1.pushPull();
  dir.pushPull();
  servo.pushPull();

  // Component: ModbusComponent
  Modbus.init();

  setup();
  sei();
  for (; ; )
  {
    loop();

    // Component: SerialComponent

    // Component: SysTickComponent
    readTimer.process();
    modbusTimer.process();

    // Component: GPIOComponent

    // Component: ModbusComponent
    Modbus.process();
  }
}