#include "TM4C123GH6PM.h"
#define GPIO_PORTF_DATA_R (*(( volatile unsigned long *)0x40025038 ) ) //5038
#include <stdint.h>
#include "delay.h"
#include <string.h>

extern void Lcd_Init(void);
extern void Lcd_Cmd(unsigned char cmnd);
extern void Lcd_Char(unsigned char data);
extern void Lcd_String(unsigned char* data);

#define LCD GPIOB   /* Define "LCD" as a symbolic name for GPIOB */
#define RS 0x20 /* PORTB BIT5 mask */
#define RW 0x40 /* PORTB BIT6 mask */
#define EN 0x80 /* PORTB BIT7 mask */
#define HIGH 1
#define LOW 0

#define clear_display     0x01
#define returnHome        0x02
#define moveCursorRight   0x06
#define moveCursorLeft    0x08
#define shiftDisplayRight 0x1C
#define shiftDisplayLeft  0x18
#define cursorBlink       0x0F
#define cursorOff         0x0C
#define cursorOn          0x0E
#define Function_set_4bit 0x28
#define Function_set_8bit 0x38
#define Entry_mode        0x06
#define Function_8_bit    0x32
#define Set5x7FontSize    0x20
#define FirstRow          0x80


void Lcd_Init()
{
	SYSCTL->RCGCGPIO = 0X22;
	GPIOB->DIR = 0XFF;
	GPIOB->DEN = 0XFF;
	delay_ms(30);

	Lcd_Cmd(0x33);
	Lcd_Cmd(0x32);
	Lcd_Cmd(0x28);
	Lcd_Cmd(0x0c);
	Lcd_Cmd(0x06);
	Lcd_Cmd(0x01);
}
void Lcd_String(unsigned char* data)
{
	int i;
	for (i = 0; data[i] != 0; i++)
	{
		Lcd_Char(data[i]);
	}
}
void Lcd_Char(unsigned char data) {
	GPIOB->DATA = (GPIOB->DATA & 0x0F) | (data & 0xF0);

	GPIOB->DATA |= (0x01);
	GPIOB->DATA |= (0X04);
	delay_ms(1);
	GPIOB->DATA &= (0XFB);
	delay_ms(1);

	GPIOB->DATA = (GPIOB->DATA & 0x0F) | (data * 16);
	GPIOB->DATA |= (0X04);
	delay_ms(1);
	GPIOB->DATA &= (0XFB);
	delay_ms(2);
	Lcd_Cmd(moveCursorRight);
}
void Lcd_Cmd(unsigned char cmnd)
{
	GPIOB->DATA = (GPIOB->DATA & 0x0F) | (cmnd & 0xF0);
	GPIOB->DATA &= (0xFE);
	GPIOB->DATA |= (0X04);
	delay_ms(1);
	GPIOB->DATA &= (0XFB);
	delay_ms(1);

	GPIOB->DATA = (GPIOB->DATA & 0x0F) | (cmnd << 4);
	GPIOB->DATA |= (0X04);
	delay_ms(1);
	GPIOB->DATA &= (0XFB);
	delay_ms(2);

}
