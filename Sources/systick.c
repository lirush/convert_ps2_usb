#include "systick.h"

volatile uint32_t TimingDelay_ms;
uint32_t Ticks=0;

void SysTickStart(uint32_t ticks)
{
  SysTick->LOAD = ticks;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
}

void SysTickStop(void)
{
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
  if (TimingDelay_ms > 0x00)
  {
    TimingDelay_ms--;
  }
	Ticks++;
}

void _delay_ms(const uint32_t ms)
{
  TimingDelay_ms = ms;
  while(TimingDelay_ms != 0);
}

uint8_t one_us=3;
uint32_t TimingDelay_us = 0;
void _delay_us(const uint32_t us)																//с частотой МК 48МГц -> эта операция ~ займет 48 циклов что соответствует при данной частоте 1 мкс
{
	TimingDelay_us = us * one_us;
	while(TimingDelay_us--);
}
