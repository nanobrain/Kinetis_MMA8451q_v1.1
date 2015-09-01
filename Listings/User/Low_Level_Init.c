/* ################Low_Level_Init.c###############################
**     Filename  : Low_Level_Init.c
**     Processor : MKL25Z
**     Date/Time : 2015-18-07, 22:38
** ###############################################################*/
#include "fsl_device_registers.h"
#include "Low_Level_Init.h"
#include "HD44780.h"

void init_CPU(void){    
/*----------------*
  Clocking Settings
	*-----------------*/	
	SIM->SCGC5 |= ( SIM_SCGC5_PORTA_MASK
								| SIM_SCGC5_PORTB_MASK
								| SIM_SCGC5_PORTC_MASK
								| SIM_SCGC5_PORTD_MASK
								| SIM_SCGC5_PORTE_MASK );

	SIM->SOPT2 |= 	SIM_SOPT2_PLLFLLSEL_MASK; 				// set PLLFLLSEL to select the PLL for this clock source 
	/*----------------*
  PWM for LEDs Registers Settings
	*-----------------*/
	PORTB->PCR[LED_R] = PORT_PCR_MUX(1);              // Set Pin B18 to PWM function
	PORTB->PCR[LED_G] = PORT_PCR_MUX(1);              // Set Pin B19 to PWM function
	PORTD->PCR[LED_B] = PORT_PCR_MUX(1);              // Set Pin D1 to GPIO function
					
	FPTB->PDDR |= (1<<LED_R);                        	// Red LED, Negative Logic (0=on, 1=off)
	FPTB->PDDR |= (1<<LED_G);                      		// Green LED, Negative Logic (0=on, 1=off)
	FPTD->PDDR |= (1<<LED_B);                        	// Blue LED, Negative Logic (0=on, 1=off)

	FPTB->PCOR |= (1<<LED_R);													// Turn off leds
	FPTB->PCOR |= (1<<LED_G);
	FPTD->PCOR |= (1<<LED_B);

	/*----------------*
  LCD Registers Settings
	*-----------------*/
	PORTE->PCR[E]  =             											// Set Pins to GPIO function
	PORTE->PCR[RS] = 																	// TODO: U can do it better !
	PORTE->PCR[RW] = 
	PORTE->PCR[D4] = 
	PORTE->PCR[D5] = 
	PORTE->PCR[D6] = 
	PORTE->PCR[D7] = PORT_PCR_MUX(1);

	PTE->PDDR	|= MASK(D4)| MASK(D5) 									// Set output
						|  MASK(D6)| MASK(D7)
						|  MASK(E) | MASK(RS)
						|  MASK(RW);
		
}

void sShort2St(char *pStr, int16_t value){
  //Ilosc cyfr konwertowanej liczby (bez uwzgledniania znaku)
  uint8_t charCount;
			 
			 if(value/10000)charCount=5;								// TODO: How to do it faster ??
	else if(value/1000)charCount=4;
	else if(value/100)charCount=3;
	else if(value/10)charCount=2;
	else charCount=1;

  if(value < 0)
	{
		(*pStr = '-');
		charCount++;
	}
  pStr = pStr + charCount;

  *pStr-- = 0x0;
	do{
			*pStr-- = abs(value % 10) + '0';
			value /= 10;
		}
	while(--charCount);
}


