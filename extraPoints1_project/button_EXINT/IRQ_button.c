#include "button.h"
#include "lpc17xx.h"
#include "../Timer/timer.h"

volatile uint8_t down0 = 0;
volatile uint8_t down1 = 0;
volatile uint8_t down2 = 0;

// Buttons interrupts handlers
void EINT0_IRQHandler (void)	  	// INT0	
{		
		disable_button(0);
	
		down0 = 1;				// flag
		enable_timer(0);	// to control bouncing and de-bouncing
	
		LPC_SC->EXTINT &= (1 << 0);     // clear pending interrupt  
}

void EINT1_IRQHandler (void)	  	// KEY1	
{
		disable_button(1);
	
		down1 = 1;				// flag
		enable_timer(0);	// to control bouncing and de-bouncing
		
		LPC_SC->EXTINT &= (1 << 1);     // clear pending interrupt 
}

void EINT2_IRQHandler (void)	  	// KEY2	
{
		disable_button(2);
	
		down2 = 1;				// flag
		enable_timer(0);	// to control bouncing and de-bouncing
		
		LPC_SC->EXTINT &= (1 << 2);     // clear pending interrupt   
}

