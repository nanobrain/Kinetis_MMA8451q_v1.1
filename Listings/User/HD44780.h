/** ###############HD44780.h##########################################
**     Filename  : HD44780.h
**     Processor : MKL25Z
**     Date/Time : 2015-10-08, 15:13
**		 Version   : 1.1
** ###################################################################
----1----2----3----4----5----6----7-10----11--12--13--14--15----16----
---Vss--Vcc--Vee---Rs---Rw---E---DB0-DB3--DB4-DB5-DB6-DB7-LED+--LED---
*/

#ifndef __HD44780_H
#define __HD44780_H

#define LCD_INIT    0x30                    // function set
#define LCD_FSET    0x28                    // 4-bit, two lines, 5x7 font
#define LCD_OFFON   0x0C                    // display on, cursor off
#define LCD_CLR     0x01                    // display clear
#define LCD_HOME    0x02                    // return cursor to home; disable shift
#define LCD_1Line		0x80										// moves cursor to first line
#define LCD_2Line   0xC0                    // moves cursor to second line
#define LCD_1Ctr    0x08                    // moves cursor to center of first line
#define LCD_2Ctr    0xC8                    // moves cursor to center of second line
#define LCD_EMODE   0x06                    // entry mode. Shift disable, cursor increment

#define E_PutVal(value)		(void)(value ? (PTE->PSOR |= MASK(E)): (PTE->PCOR |= MASK(E)));
#define RS_PutVal(value)	(void)(value ? (PTE->PSOR |= MASK(RS)): (PTE->PCOR |= MASK(RS)));
#define RW_PutVal(value)	(void)(value ? (PTE->PSOR |= MASK(RW)): (PTE->PCOR |= MASK(RW)));
#define Set_Input  				PTE->PDDR &= ~MASK(D4) & ~MASK(D5) & ~MASK(D6) & ~MASK(D7)
#define Set_Output 				PTE->PDDR |=  MASK(D4) |  MASK(D5) |  MASK(D6)|  MASK(D7)

#define D4	(22) // PORT E
#define D5	(23)
#define D6	(29)
#define D7	(30)
#define E		(21)
#define RS	(5)
#define RW	(20)
#define MASK(x) (1UL << (x))

extern char LCD_LINE1[16];
extern char LCD_LINE2[16];

void LCD_Update(void);
void LCD_SetInput(void);
void LCD_SetOutput(void);
uint8_t LCD_ReadNibble(void);
uint8_t LCD_CheckStatus(void);
void LCD_WriteNibble(char Data);
void LCD_WriteData(char DataToWrite);
void LCD_SetCursor(char line, char col);
void LCD_WriteText(char* text);
void LCD_WriteCommand(char command);
void LCD_Init(void);
#endif
