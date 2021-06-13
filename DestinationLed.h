#include "math.h"
#include "TM4C123GH6PM.h"
#define GPIO_PORTF_DATA_R (*(( volatile unsigned long *)0x40025038 ) ) //5038

const float delta = 0.00001;
extern void  Destination_Reached(float distance, float longcurr, float latcurr, float longend, float latend);
void Destination_Reached(float distance, float longcurr, float latcurr, float longend, float latend)
{
	float longdiff = fabs(longend - longcurr);
	float latdiff = fabs(latend - latcurr);
	if (distance > 100 || ((longdiff < delta) && (latdiff < delta)))
	{
		GPIO_PORTF_DATA_R |= 0x02;
	}
}
