/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../adc/adc.h"
#include "../Pong/Paddle/paddle.h"
#include "../Pong/Ball/ball.h"
#include "../Pong/pong_model.h"
#include "../button_EXINT/button.h"

uint16_t SinTable[45] =                                       
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern uint8_t down0;
extern uint8_t down1;
extern uint8_t down2;

extern Ball ball;

void TIMER0_IRQHandler (void)		// to control buttons
{						
		static uint8_t is_pause = 0;		// boolean variable
		
		// buttons bouncing management
		
		if (down0 > 1)
		{
				if((LPC_GPIO2->FIOPIN & (1<<10)) == 0)	// INT0 button is pressed
				{
						if(down0 == 2)
						{
								down0++;
							
								reset_game();
						}
				}
				else 		// INT0 button has been released
				{
						down0 = 0;
						disable_timer(0);
						reset_timer(0);
					
						//re-enable button interrupts
						enable_button(0);
				}
		}
		else if (down0 == 1)
				down0++;
		
		if (down1 > 1)
		{
				if((LPC_GPIO2->FIOPIN & (1<<11)) == 0)	// KEY1 button is pressed
				{
						if(down1 == 2)
						{
								down1++;
															
								start_game();
						}
				}
				else 		// KEY1 button has been released
				{
						down1 = 0;
						disable_timer(0);
						reset_timer(0);
					
						//re-enable button interrupts
						enable_button(1);
				}
		}
		else if (down1 == 1)
				down1++;
		
		if (down2 > 1)
		{
				if((LPC_GPIO2->FIOPIN & (1<<12)) == 0)	// KEY2 button is pressed
				{
						if(down2 == 2)
						{
								down2++;
								
								// action button 1 -> pause on/off
								is_pause ^= 1;	// 0 -> 1 -> 0 -> 1 -> ...
								
								if(is_pause) 
								{
										// disable INT0 and KEY1 buttons interrupts
										disable_button(0);
										disable_button(1);
						
										pause();
								}
								else
								{
										// enable INT0 and KEY1 buttons interrupts
										enable_button(0);
										enable_button(1);
									
										clear_pause();
								}
						}
				}
				else 		// KEY2 button has been released
				{
						down2 = 0;
						disable_timer(0);
						reset_timer(0);
					
						// re-enable button interrupts
						enable_button(2);
				}
		}
		else if (down2 == 1)
				down2++;
		
		LPC_TIM0->IR = 1;			/* clear interrupt flag */
		return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)	// to control potentiometer & ball movements
{
		// potentiometer A/D conversion management
		if(paddle_has_to_be_shown())
				ADC_start_conversion();		// measuring voltmeter voltage
			
		if(ball_has_to_be_moved())
				if(change_ball_position(&ball))
						draw_ball(&ball);
		
		LPC_TIM1->IR |= (1<<0);			/* clear interrupt flag of MR0 */
		return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
volatile int ticks = 0;

void TIMER2_IRQHandler (void)	// to play sounds
{	
		/* DAC management */	
		LPC_DAC->DACR = SinTable[ticks] << 6;
		ticks++;
		
		if(ticks == 45)
		{
				ticks = 0;
				disable_timer(2);
		}
		
		LPC_TIM2->IR |= (1<<0);			/* clear interrupt flag of MR0 */
		return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
