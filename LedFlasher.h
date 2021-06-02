#include "TM4C123GH6PM.h"
#define GPIO_PORTF_DATA_R (*(( volatile unsigned long *)0x40025038 ) ) //5038
#include "delay.h"

extern void LedFlash();
void LedFlash()
{
	for (int i = 2; i < 16; i += 2)
	{
		GPIO_PORTF_DATA_R = i;
		delay_ms(1000);
	}
}