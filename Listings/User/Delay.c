/* ###############Delay.c##########################################
**     Filename  : Delay.c
**     Processor : MKL25Z
**     Date/Time : 2015-18-07, 22:38
** ###############################################################*/
#include "fsl_device_registers.h"

/*
 * Just simple delay // Why it is not exact ?
 */
void _delay_ms(unsigned delayTicks){    
	delayTicks *=(uint64_t)SystemCoreClock/10000u;
    while(delayTicks--)
    {
        __ASM("nop"); // <- How to check how long does it take to do No-op on mkl25z ?
    }
}
