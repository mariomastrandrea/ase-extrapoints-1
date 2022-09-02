
#include "button.h"
#include "lpc17xx.h"

/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {

		LPC_PINCON->PINSEL4		 &= ~(1 << 21);
		LPC_PINCON->PINSEL4    |=  (1 << 20);		 /* External interrupt 0 pin selection */
		LPC_GPIO2->FIODIR      &= ~(1 << 10);    /* PORT2.10 defined as input          */

		LPC_PINCON->PINSEL4		 &= ~(1 << 23);
		LPC_PINCON->PINSEL4    |=  (1 << 22);    /* External interrupt 0 pin selection */
		LPC_GPIO2->FIODIR      &= ~(1 << 11);    /* PORT2.11 defined as input          */
		
		LPC_PINCON->PINSEL4		 &= ~(1 << 25);
		LPC_PINCON->PINSEL4    |=  (1 << 24);    /* External interrupt 0 pin selection */
		LPC_GPIO2->FIODIR      &= ~(1 << 12);    /* PORT2.12 defined as input          */

		LPC_SC->EXTMODE = 0x7;

		NVIC_EnableIRQ(EINT2_IRQn);              /* enable irq in nvic                 */
		NVIC_SetPriority(EINT2_IRQn, 1);				 /* priority, the lower the better     */
		NVIC_EnableIRQ(EINT1_IRQn);              /* enable irq in nvic                 */
		NVIC_SetPriority(EINT1_IRQn, 2);				 
		NVIC_EnableIRQ(EINT0_IRQn);              /* enable irq in nvic                 */
		NVIC_SetPriority(EINT0_IRQn, 3);				 /* decreasing priority	from EINT2->0	 */
}


void enable_button (uint8_t num_button)
{
		switch(num_button)
		{
				case 0:		// enable button INT0
					NVIC_EnableIRQ(EINT0_IRQn);						 // enable Button interrupts	
					LPC_PINCON->PINSEL4  |= (1 << 20);     // EINT0 pin selection 
					break;
				
				case 1:		// enable button KEY1
					NVIC_EnableIRQ(EINT1_IRQn);						 // enable Button interrupts	
					LPC_PINCON->PINSEL4  |= (1 << 22);     // EINT1 pin selection 
					break;
					
				case 2:		// enable button KEY2
					NVIC_EnableIRQ(EINT2_IRQn);						 // enable Button interrupts	
					LPC_PINCON->PINSEL4  |= (1 << 24);     // EINT2 pin selection 
					break;
		}
}

void disable_button(uint8_t num_button)
{
		switch(num_button)
		{
				case 0:		// disable button INT0
					NVIC_DisableIRQ(EINT0_IRQn);							// disable Button interrupts	
					LPC_PINCON->PINSEL4    &= ~(1 << 20);     // GPIO pin selection 
					break;
				
				case 1:		// disable button KEY1
					NVIC_DisableIRQ(EINT1_IRQn);							// disable Button interrupts	
					LPC_PINCON->PINSEL4    &= ~(1 << 22);     // GPIO pin selection 
					break;
					
				case 2:		// disable button KEY2
					NVIC_DisableIRQ(EINT2_IRQn);							// disable Button interrupts	
					LPC_PINCON->PINSEL4    &= ~(1 << 24);     // GPIO pin selection 
					break;
		}
}
