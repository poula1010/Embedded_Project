#include "TM4C123GH6PM.h"
#include "delay.h"
#define GPIO_PORTF_DATA_R (*(( volatile unsigned long *)0x40025038 ) ) //5038
extern void Initialize();
void InitializeF()
{
	SYSCTL->RCGCGPIO = 0X20;
	delay_ms(2);
	GPIOF->DIR = 0X0E;
	GPIOF->DEN = 0X0E;
	delay_ms(2);
}
