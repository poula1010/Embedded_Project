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

float LongCurr;
float Latcurr;
float Latprev;
float Longprev;
float distance;
float NMEAToNorm(float x);

int starter;

void UART5_Transmitter(unsigned char data);
void LongLatFetch(void);
void LongLatbuff(void);
void BuffToFloat(void);
void UARTInitialize(void);

int main()
{
	initialize();
  	UARTInitialize();
	starter =0;
	distance = 0.f;
	inputmode = 1;
	Lcd_Init();
	delay_ms(20);
		while (1)
		{
			keypad_Init();
			if (inputmode)
			{
				Lcd_Char(keypad_getkey);
			}
			else
			{
				LongLatFetch();
				LongLatbuff();
				BuffToFloat();
				distance += Distance_Calculator(Longprev,Latprev,LongCurr,Latcurr);
				char lcdbuff[10];
        			To_str(distance,lcdbuff);
				Lcd_String("distance=");
				Lcd_String(lcdbuff);
			}
			//LedFlash();
	}
}

float NMEAToNorm(float x)
{
	int big = x/100;
	float small = (x-100*big)/60.f;
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
oid LongLatbuff()
{
	int indexer =0;
	const char s[2] = ",";
		if(Result[0] != 0)
		{
    char* token = strtok(Result,s);
		
    while(token != NULL)
    {
			if(indexer == 3)
        {
            for(int i =0;token[i] !=0 ;i++)
            {
            Latbuff[i] = token[i];
            }
        }
                if(indexer == 5)
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
	//int indexer =0;
		if(c0 == '$')
		{
			char c1 = UART5_Receiver();
			if(c1 == 'G')
			{
				char c2 = UART5_Receiver();
				{
					if(c2 == 'P')
					{
						char c3 = UART5_Receiver();
						if(c3 =='R')
						{
							char c4 = UART5_Receiver();
							if(c4 == 'M')
							{
								char c5 = UART5_Receiver();
								if(c5 == 'C')
								{
									char c6 = UART5_Receiver();
									if(c6 == ',')
									{
										char c7 = UART5_Receiver();
										if(c7 == ',')
										{
											//printstring("disabled");
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
char UART5_Receiver(void)  
{
    char data;
	  while((UART5->FR & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART5->DR ;  	/* before giving it another byte */
    return (unsigned char) data; 
}

void UART5_Transmitter(unsigned char data)  
{
    while((UART5->FR & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART5->DR = data;                  /* before giving it another byte */
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
