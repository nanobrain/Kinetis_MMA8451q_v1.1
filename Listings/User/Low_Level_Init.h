/** ###############Low_Level_Init.h#####################################
**     Filename  : Low_Level_Init.h
**     Processor : Universal
**     Date/Time : 2015-31-05, 20:34
** ###################################################################*/

#define MASK(x) (1UL << (x))
#define LED_R 18
#define LED_G 19
#define LED_B 1

#define AVGfactor 100

void _delay_ms(unsigned delayTicks);
static void init_hardware(void);
void RGB(uint16_t Red, uint16_t Green, uint16_t Blue);				// RGB-LED Control: 1=on, 0=off, for each of the 3 colors
void Hello_RGB(void);

void SysTick_Handler(void);
void Delay_us(uint32_t dlyTicks);
void Delay_ms(uint32_t dlyTicks);

void init_CPU(void);
void sShort2St(char *pStr, int16_t value);
