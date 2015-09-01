/** ###############HD44780.c###########################################
**     Filename  : HD44780.c
**     Processor : MKL25Z
**     Date/Time : 2015-10-08, 15:13
**		 Version   : 1.1
** ###################################################################
----1----2----3----4----5----6----7-10----11--12--13--14--15----16--LCD
-------------------E05--E20--E21-NC--NC---E22-E23-E29-E30-----------Kinetis
---Vss--Vcc--Vee---Rs---Rw---E---DB0-DB3--DB4-DB5-DB6-DB7-LED+--LED-
*/
#include "fsl_device_registers.h"
#include "Low_Level_Init.h"
#include "HD44780.h"
#include <cmsis_os.h>                                           // CMSIS RTOS header file

char LCD_LINE1[16];
char LCD_LINE2[16];

void LCD_Update()
{
	LCD_WriteCommand(LCD_CLR);
	LCD_WriteText(LCD_LINE1);
	LCD_SetCursor(2,0);
	LCD_WriteText(LCD_LINE2);
	
}
void LCD_IO_PutVal(char val){
	
	(void)((val & MASK(0)) ? (PTE->PSOR |= MASK(D4)): (PTE->PCOR |= MASK(D4)));
	(void)((val & MASK(1)) ? (PTE->PSOR |= MASK(D5)): (PTE->PCOR |= MASK(D5)));
	(void)((val & MASK(2)) ? (PTE->PSOR |= MASK(D6)): (PTE->PCOR |= MASK(D6)));
	(void)((val & MASK(3)) ? (PTE->PSOR |= MASK(D7)): (PTE->PCOR |= MASK(D7)));
}

uint8_t LCD_IO_GetVal(){

	uint8_t temp;
	
	(void)((PTE->PDIR & MASK(D7)) ? (temp |=MASK(3)): (temp &=~MASK(3)));
	(void)((PTE->PDIR & MASK(D6)) ? (temp |=MASK(2)): (temp &=~MASK(2)));
	(void)((PTE->PDIR & MASK(D5)) ? (temp |=MASK(1)): (temp &=~MASK(1)));
	(void)((PTE->PDIR & MASK(D4)) ? (temp |=MASK(0)): (temp &=~MASK(0)));
	
	return temp;
}

uint8_t LCD_ReadNibble(void) {
	
	uint8_t temp=0;
	
	Set_Input;
	E_PutVal(1);
	temp = LCD_IO_GetVal();
	E_PutVal(0);
	Set_Output;	
	return temp;
}
  
uint8_t LCD_CheckStatus(void){
	
	uint8_t status = 0;
	RS_PutVal(0);
	RW_PutVal(1);
	status |= (LCD_ReadNibble() << 4);
	status |=  LCD_ReadNibble();
	return status;

}

void LCD_WriteNibble(char Data){
	
	E_PutVal(1);
	LCD_IO_PutVal(Data);
	E_PutVal(0);

}

void LCD_WriteData(char DataToWrite){

	RS_PutVal(1);
	RW_PutVal(0);
	
	LCD_WriteNibble(DataToWrite >> 4);
	LCD_WriteNibble(DataToWrite & 0x0F);
	while(LCD_CheckStatus() & 0x80);
}

void LCD_WriteText(char * text) {	

	while(*text) {
	  LCD_WriteData(*text);
	  text++;
	}
	osDelay(1);
}

void LCD_WriteCommand(char command){
	
	E_PutVal(0);
	RS_PutVal(0);
	RW_PutVal(0); 
	LCD_WriteNibble(command >> 4);
	LCD_WriteNibble(command & 0x0F);
	osDelay(1);
}

void LCD_SetCursor(char line, char col){
	uint16_t buffer = (line==1)? (LCD_1Line + col):(LCD_2Line + col);
	LCD_WriteCommand(0xFE);
	LCD_WriteCommand(buffer);
}

void LCD_Init(void){

	osDelay(50);														// 50ms dla ustabilizowania siê napiêcia zasilania
	
																						// start initialization	                                                 
	LCD_WriteCommand((uint8_t)LCD_INIT);
	osDelay(1);                             // 1ms, minimum 39us
	
	LCD_WriteCommand((uint8_t)LCD_FSET);
	osDelay(1);                             // 1ms, minimum 39us
	
	LCD_WriteCommand((uint8_t)LCD_FSET);
	osDelay(1);                             // 1ms, minimum 37us
	
	LCD_WriteCommand((uint8_t)LCD_OFFON);
	osDelay(1);                             // 1ms, minimum 37us
	
	LCD_WriteCommand((uint8_t)LCD_CLR);
	osDelay(2);                             // 2ms, minimum 1.54ms
	
	LCD_WriteCommand((uint8_t)LCD_EMODE);
	osDelay(1);                             // 1ms (O tym dokumentacja nie wspominala....)
	
	                                          // stop initialization
}
