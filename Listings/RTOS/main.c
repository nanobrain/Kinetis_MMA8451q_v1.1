/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "fsl_device_registers.h"
#include "Low_Level_Init.h"
#include "I2C.h"
#include "HD44780.h"

extern int Init_HD44780_Th(void);
extern int Init_MMA8451Q_Th(void);

/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	init_CPU();
	I2C0_Init();
	LCD_Init();
	Init_HD44780_Th();
	Init_MMA8451Q_Th();

	osKernelStart ();                         // start thread execution 
}
