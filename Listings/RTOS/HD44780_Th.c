#include <cmsis_os.h>                                           		// CMSIS RTOS header file
#include "Low_Level_Init.h"
#include "HD44780.h"
#include "LED.h"
#include <string.h>
#include "MMA8451Q_Drv.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

void HD44780_Th (void const *argument);                             // thread function
osThreadId tid_HD44780_Th;                                          // thread id
osThreadDef (HD44780_Th, osPriorityNormal, 1, 0);                   // thread object

extern char 		LCD_LINE1[16];
extern char 		LCD_LINE2[16];
			 char			bufferX[7];
			 char			bufferY[7];
			 char			bufferZ[7];
			 uint16_t	cnt=0;
extern osMailQId mail_box;                                        // mail queue id
extern Accel_Status __Status;

int Init_HD44780_Th (void) {

  tid_HD44780_Th = osThreadCreate (osThread(HD44780_Th), NULL);
  if(!tid_HD44780_Th) return(-1);
  return(0);
}

void HD44780_Th (void const *argument) {
	mail_format* bufferRx;
	osEvent evt;
  while (1) {
		//osSignalWait(Accel_DR,1);
		evt = osMailGet(mail_box,osWaitForever);
		if(evt.status == osEventMail)
		{
			bufferRx = (mail_format*) evt.value.p;
			
			if(cnt%4==0)RGB(0,1,0);

			Accel_s14FracOut(&__Status, bufferX, bufferRx->x_Value);
			Accel_s14FracOut(&__Status, bufferY, bufferRx->y_Value);
			Accel_s14FracOut(&__Status, bufferZ, bufferRx->z_Value);
			
			strcpy(LCD_LINE1," x=");
			strcat(LCD_LINE1,bufferX);
			
			strcpy(LCD_LINE2,"y=");
			strcat(LCD_LINE2,bufferY);
			
			strcat(LCD_LINE2,"z=");
			strcat(LCD_LINE2,bufferZ);
			
			osMailFree(mail_box,bufferRx);
			LCD_Update();
			RGB_CLEAR;
			cnt++;
		}
		osThreadYield();                                            // suspend thread
		
  }
}

