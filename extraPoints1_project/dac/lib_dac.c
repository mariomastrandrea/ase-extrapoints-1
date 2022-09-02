#include "lpc17xx.h"

/*----------------------------------------------------------------------------
  Function that initializes DAC
 *----------------------------------------------------------------------------*/
void DAC_init (void) {
		
		LPC_PINCON->PINSEL1 |= (1<<21);			/* set P0.26 as AOUT (configuration: 10)*/
		LPC_PINCON->PINSEL1 &= ~(1<<20);
	
		LPC_GPIO0->FIODIR |= (1<<26);		/* set P0.26 as output */
}
