#include <cmsis_os.h>                                           				// CMSIS RTOS header file
#include "Low_Level_Init.h"
#include "MMA8451Q_Drv.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void MMA8451Q_Th (void const *argument);                             		// thread function

void Timer_Callback(void const* arg);
void CreateTimer(uint32_t argument);
osTimerDef(Timer,Timer_Callback);

osThreadId 		tid_MMA8451Q_Th;                                          // thread id
osThreadDef 	(MMA8451Q_Th, osPriorityNormal, 1, 0);                   	// thread object
uint32_t 			param;
extern osThreadId tid_HD44780_Th;

Accel_Status* __Status;

osMailQId mail_box;                                        // mail queue id
osMailQDef (mail_box, mail_obj, mail_format);              // mail queue object

int Init_MMA8451Q_Th (void) {
	
	mail_box = osMailCreate(osMailQ(mail_box), NULL);        // create mail queue
  tid_MMA8451Q_Th = osThreadCreate (osThread(MMA8451Q_Th), NULL);
  if(!tid_MMA8451Q_Th) return(-1);
  return(0);
}

void MMA8451Q_Th (void const *argument) {
	//Accel_Actv();																		//IMPORTANT: TODO: Use init function
	//Accel_WriteReg(MMA8451_CTRL_REG1, 0x1D); 				// Output Data Rate = 100 Hz
																										// Reduced noise mode
																										// Set ACTIVE mode
																										// TODO: Use own functions
	Accel_Actv_Mode(M_2G);
	param = 150;
	CreateTimer(param);

  while (1) {
		
    osThreadYield();                                            					// suspend thread
  }
}

void Timer_Callback(void const* arg){
	mail_format* bufferTx=0;
	bufferTx = (mail_format*)osMailAlloc(mail_box, osWaitForever); //allocate a mailslot
	bufferTx->x_Value = Accel_ReadAxis(X);
	bufferTx->y_Value = Accel_ReadAxis(Y);
	bufferTx->z_Value = Accel_ReadAxis(Z);
	osMailPut(mail_box, bufferTx);
	//osSignalSet(tid_HD44780_Th,Accel_DR);
	
}

void CreateTimer(uint32_t argument){
	osTimerId id_Timer = osTimerCreate(osTimer(Timer),osTimerPeriodic,&argument);
	if(id_Timer)
	{
		//Timer Created
		osStatus status;
		status = osTimerStart(id_Timer,argument);
		
		if(status != osOK)
		{
			//ERROR
			RGB(1,0,0);
		}
	}
}
