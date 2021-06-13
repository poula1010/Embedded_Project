#include "TM4C123GH6PM.h"
#define GPIO_PORTF_DATA_R (*(( volatile unsigned long *)0x40025038 ) ) //5038
#include "delay.h"
#include "LCD.h"
#include "DistanceCalc.h"
#include "DestinationLed.h"
#include "initialize.h"
#include "to_str.h"
#include "LedFlasher.h"
#include "stdbool.h"
#include "stdlib.h"

bool inputmode;

char Result[50];
char Longbuff[12];
char Latbuff[12];

int currentmode;
int starter;

double LongCurr;
double Latcurr;
double Latprev;
double Longprev;
double distance;
double NMEAToNorm(float x);

unsigned char UART5_Receiver(void);
void LongLatFetch(void);
void LongLatbuff(void);
void BuffToFloat(void);
void UARTInitialize(void);
char c[100];
int main()
{
	InitializeF();
	currentmode = 1;
	Lcd_Init();
	delay_ms(20);
	
	UARTInitialize();
	starter =0;
	distance =0.01;
	while(1)
	{	
		if(distance>100)
				{
					GPIO_PORTF_DATA_R =0X02;
				}	
	/*	for(int i =0; i<100;i++)
		{
		 c[i]= UART5_Receiver();
		}
		for (int i =0;i<100;i++)
		{
			if(i%16 ==0)
			{
				Lcd_Cmd(0x01);
			}
			Lcd_Char(c[i]);
			delay_ms(100);
		}*/
				//LongLatFetch();
		/*for (int i =0;i<50;i++)
		{
			if(i%16 ==0)
			{
				Lcd_Cmd(0x01);
			}
			Lcd_Char(Result[i]);
			delay_ms(100);
		}*/
			LongLatFetch();
				LongLatbuff();
				BuffToFloat();
				double distanceadded = Distance_Calculator(Longprev,Latprev,LongCurr,Latcurr);
				if(distanceadded >= 0.00001)
				{
					distance+= distanceadded;
				char lcdbuff[10];
        			To_str(distance,lcdbuff);
	
				Lcd_Cmd(0x01);
				//Lcd_String("distance=");
				Lcd_String(lcdbuff);				
				delay_ms(4000);
				}
		
	}
	//inputmode =1;
	//keypad_Init();
	

}
double NMEAToNorm(float x)
{
	int big = x/100;
	double small = (x-100*big)/60.0;
	return big+small;
}
void BuffToFloat()
{
	if(starter ==0)
	{
		LongCurr = NMEAToNorm(atof(Longbuff));
		Latcurr = NMEAToNorm(atof(Latbuff));
		Longprev = LongCurr;
		Latprev = Latcurr;
		starter +=1;
	}
	else
	{
		Longprev = LongCurr;
		Latprev = Latcurr;
		LongCurr = NMEAToNorm(atof(Longbuff));
		Latcurr = NMEAToNorm(atof(Latbuff));
	}

}

/* LCD and GPIOB initialization Function */ 
void LongLatbuff()
{
	int indexer =0;
	const char s[2] = ",";
		if(Result[0] != 0)
		{
    char* token = strtok(Result,s);
		
    while(token != NULL)
    {
			if(indexer == 2)
        {
            for(int i =0;token[i] !=0 ;i++)
            {
            Latbuff[i] = token[i];
            }
        }
                if(indexer == 4)
        {
            for(int i =0;token[i]!= 0;i++)
            {
            Longbuff[i] = token[i];
            }
        }
        indexer +=1;
    
      token = strtok(NULL, s);
    }
	}
}
void LongLatFetch()
{
 //char c0,GPSValues[100],latitudeResult[10],longitudeResult[10],parseValue[12][20],*token,tarih[9],*saat,guncelSaat[9];
    //double latitude=0.0,longitude=0.0,seconds=0.0,result=0.0,minutes=0.0;
	//const char comma[2] = ",";
    //int index=0,degrees,i=0,j=0;
	char c0 = UART5_Receiver();
		if(c0 == '$')
		{
					//Lcd_Char(c0);
			char c1 = UART5_Receiver();
			if(c1 == 'G')
			{
						//Lcd_Char(c1);
				char c2 = UART5_Receiver();
				{
					if(c2 == 'P')
					{
						//Lcd_Char(c2);
						char c3 = UART5_Receiver();
						if(c3 =='R')
						{
								//	Lcd_Char(c3);
							char c4 = UART5_Receiver();
							if(c4 == 'M')
							{
										//Lcd_Char(c4);
								char c5 = UART5_Receiver();
								if(c5 == 'C')
								{
									//Lcd_Char(c5);
									//delay_ms(2000);
									char c6 = UART5_Receiver();
									if(c6 == ',')
									{
										char c7 = UART5_Receiver();
										if(c7 == ',')
										{
													Lcd_Cmd(0x01);
													Lcd_String("disabled");
													delay_ms(1000);
										}
										else
										{
											for(int i =0; i<50;i++)
											{
												Result[i]= UART5_Receiver();
											}
											//printstring(Result);
										}
									}
								}
							}
						}
					}
				}
			}
		}
}
unsigned char UART5_Receiver(void)  
{
    char data;
	  while((UART5->FR & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART5->DR ;  	/* before giving it another byte */
    return (unsigned char) data; 
}

/*void printstring(char *str)
{
  while(*str)
	{
		UART5_Transmitter(*(str++));
	}
}*/

void UARTInitialize()
{
	
	SYSCTL->RCGCUART |= 0x20;  /* enable clock to UART5 */
    SYSCTL->RCGCGPIO |= 0x10;  /* enable clock to PORTE for PE4/Rx and RE5/Tx */
    delay_ms(1);
    /* UART0 initialization */
    UART5->CTL = 0;         /* UART5 module disbable */
    UART5->IBRD = 104;      /* for 9600 baud rate, integer = 104 */
    UART5->FBRD = 11;       /* for 9600 baud rate, fractional = 11*/
    UART5->CC = 0;          /*select system clock*/
    UART5->LCRH = 0x60;     /* data lenght 8-bit, not parity bit, no FIFO */
    UART5->CTL = 0x301;     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    GPIOE->DEN = 0x30;      /* set PE4 and PE5 as digital */
    GPIOE->AFSEL = 0x30;    /* Use PE4,PE5 alternate function */
    GPIOE->AMSEL = 0;    /* Turn off analg function*/
    GPIOE->PCTL = 0x00110000;     /* configure PE4 and PE5 for UART */
	
	  delay_ms(1); 
}
/* LCD and GPIOB initialization Function */ 
/*while (1)
		{
			//LedFlash();
			if(inputmode)
			{
				if(indexer ==0)
				{
				Lcd_Cmd(0x01);
				Lcd_Cmd(0x0F);
				Lcd_String("LongD=");
				}
				char test = keypad_getkey();
				char buffy[12];

				if(test != NULL)
				{
					if(test != 'l')
					{
						buffy[indexer]=test;
						indexer+=1;
						Lcd_Char(test);
						delay_ms(2000);
					}
					else
					{
						Lcd_Cmd(0x10);
						indexer-=1;
						delay_ms(2000);
					}
				}
				
			}
			if(!inputmode)
			{
						if(currentmode ==0)
					{
						Lcd_Cmd(0x01);
						//delay_ms(10);
					  Lcd_String("LongC:");
						char data[12];
						To_str(12.8895,&data[0]);
						Lcd_String(data);
						Lcd_Line2();
						To_str(53.5695,&data[0]);
						Lcd_String("LatC:");
						Lcd_String(data);
					}
											if(currentmode ==1)
					{
						Lcd_Cmd(0x01);
						//delay_ms(10);
					  Lcd_String("Distance=:");
					}
				char test = keypad_getkey();
				if(test != NULL)
				{
				if(test == 'r' && currentmode ==0)
				{
					currentmode=1;
					delay_ms(2000);
				}
				else if(test == 'r'  && currentmode ==1)
				{
					currentmode =0;
					delay_ms(2000);
			}
				}
				
			}
	}
}

*/		
