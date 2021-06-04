#include "TM4C123GH6PM.h"
#include "delay.h"
#define  RowsSize  5
#define  ColsSize  4

extern void keypad_Init();
extern char keypad_getkey(bool inputmode);
unsigned  char symbol[RowsSize][ColsSize] = { { 'F', 'G',  '#', '*'},
                                                { '1', '2',  '3', 'u'},
                                                { '4', '5',  '6', 'd'},
                                                { '7', '8',  '9', 'E'},
                                                { 'l', '0',  'r', 'e'} };

void keypad_Init()
{
    SYSCTL->RCGCGPIO |= 0x11;        //Enable clock to PORTA and PORTE  
    delay_ms(2);  //wait for clock to be setted
   // GPIOC->CR |= 0xF0;             //Allow settings for all pins of PORTC
    GPIOE->CR |= 0x1E;             //Allow settings for all pins of PORTD
    GPIOE->DIR |= 0x00;             //PE1-PE4 are used with row and set them as digital input pins
    GPIOE->PDR |= 0x1E;             //Enable pull down resistor on PORTE
    GPIOA->DIR = 0xFF;
    GPIOA->DEN |= 0xF0;             //Set PORTC as digital pins
    GPIOE->DEN |= 0x1E;             //Set PORTE as digital pins
}
char keypad_getkey()
{
    while (1)
    {
        for (int i = 0; i < 4; i++)    //Scan columns loop
        {
            GPIOA->DATA = (1U << i + 4);
            delay_ms(1);
            for (int j = 0; j < 4; j++)  //Scan rows
            {
                if ((GPIOE->DATA & 0x1E) & (1U << j + 1))
                    return symbol[j][i];
            }
        }
    }
}