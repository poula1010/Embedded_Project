#include "TM4C123GH6PM.h"
#ifndef          delay_H
#define          delay_H

#define 					XTAL						16000000         //CPU frequency 
#define					load_val				(XTAL/1000)
#define					calib_m  					1	   //

#include <stdint.h>

extern void delay_ms(uint32_t delay);
void delay_ms(uint32_t delay) {
	for (; delay > 0; --delay) {
		SysTick->LOAD = load_val - calib_m;
		SysTick->CTRL |= 0x5;
		while ((SysTick->CTRL & (1 << 16)) == 0);
	}
	SysTick->CTRL = 0;
}
#endif