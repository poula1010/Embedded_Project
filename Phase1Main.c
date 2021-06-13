#include "TM4C123GH6PM.h"
#define GPIO_PORTF_DATA_R (*(( volatile unsigned long *)0x40025038 ) ) //5038
#include "delay.h"
#include "LCD.h"
#include "DistanceCalc.h"
#include "DestinationLed.h"
#include "initialize.h"
#include "to_str.h"
#include "LedFlasher.h"
#include "keypad.h"
bool inputmode;
float distanceCovered;
int main()
{
	initialize();
	inputmode = 1;
	Lcd_Init();
	delay_ms(20);

	distanceCovered = 10;

	Lcd_String("distance");
	Lcd_Cmd(moveCursorRight);
	Lcd_String(To_string(distanceCovered));
		delay_ms(100);
		while (1)
		{
			keypad_Init();
			if (inputmode)
			{
				Lcd_Char(keypad_getkey);
			}
			LedFlash();
	}
}



/* LCD and GPIOB initialization Function */ 
