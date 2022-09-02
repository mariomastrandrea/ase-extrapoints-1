/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../Pong/Paddle/paddle.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;
unsigned short AD_last = 0xFF;     		// last converted value  
extern Paddle paddle;

unsigned short read_AD_value (void);

void ADC_IRQHandler(void) {
  	
		AD_current = read_AD_value();	/* Read Conversion Result  */
		// AD_current contains a value from 0 to 4095 (12 bit)
	
		if(AD_current != AD_last){
				// update paddle position by means of current AD value, then drow the paddle
				if(set_paddle_position(&paddle, AD_current, 0xFFF))
					draw_paddle(&paddle);
				
				AD_last = AD_current;
		}
}

unsigned short read_AD_value (void)
{
		return ((LPC_ADC->ADGDR>>4) & 0xFFF);
}
