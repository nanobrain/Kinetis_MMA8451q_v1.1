/* ################LED.c##########################################
**     Filename  : LED.c
**     Processor : MKL25Z
**     Date/Time : 2015-18-07, 22:38
** ###############################################################*/
#include <cmsis_os.h>                                           // CMSIS RTOS header file
#include "Low_Level_Init.h"
#include "LED.h"
#include "Delay.h"


void RGB(uint16_t Red,uint16_t Green,uint16_t Blue){
  (void)(Red   ? (FPTB->PCOR |= MASK(LED_R)) : (FPTB->PSOR |= MASK(LED_R)));
	(void)(Green ? (FPTB->PCOR |= MASK(LED_G)) : (FPTB->PSOR |= MASK(LED_G)));
	(void)(Blue  ? (FPTD->PCOR |= MASK(LED_B)) : (FPTD->PSOR |= MASK(LED_B)));
}

void Hello_RGB()
{
	RGB(1,0,0);		
	osDelay(200);
	RGB(0,1,0);
	osDelay(200);
	RGB(0,0,1);
	osDelay(200);
	RGB(0,0,0);
}
