/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "button_EXINT/button.h"
#include "adc/adc.h"
#include "dac/dac.h"
#include "Pong/pong_model.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


int main(void)
{
		uint32_t Timer0_MR0, Timer1_MR0;
		uint32_t _50ms = 0x1312D0;	// 50ms * 25MHz =  1 250 000 = 0x1312D0
	
		// initializations
		SystemInit();  									/* System Initialization (i.e., PLL)  */
	
		// buttons
		BUTTON_init();
	
		// ADC & DAC - Potentiometer & Speaker
		ADC_init();
		DAC_init();
	
		// LCD & Touch Panel
		LCD_Initialization();
		//TP_Init();
		//TouchPanel_Calibrate();					// touch-screen calibration
			
		// Timers
		
		// 	-	TIMER0 -> (50ms) to control bouncing and de-bouncing; active only after pressing a button
		Timer0_MR0 = _50ms;				
		init_timer(0, Timer0_MR0, 0);				
			
		//  - TIMER1 -> (50ms) to control potentiometer & ball movements; always active during game
		Timer1_MR0 = _50ms; 
		init_timer(1, Timer1_MR0, 0);			
		enable_timer(1);
		
		// - TIMER2 -> to play sounds; active only when playing sound
		
		/* START THE PONG GAME HERE */
		initialize_game_field();
		reset_game();							
		
		LPC_SC->PCON |= 0x1;									/* power-down	mode	*/
		LPC_SC->PCON &= ~(0x2);						
		
		while (1)	{
				__ASM("wfi");
		}
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
