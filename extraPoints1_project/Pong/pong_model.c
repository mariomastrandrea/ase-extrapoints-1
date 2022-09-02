/******************************************************
*				NOTE: axis x and y directions below
*	 	|		
*	 -0-------->	x-axis
*		|	
*		|
*		|	
*		v											unit of measure is pixel (px)
*			y-axis
*
*	This means that angles are defined *clockwise* starting from x-axis origin
*
******************************************************/

#include <stdio.h>
#include <string.h>
#include "pong_model.h"
#include "Ball/Ball.h"
#include "Paddle/paddle.h"
#include "lib_game_display_utilities.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../button_EXINT/button.h"
#include "lib_game_sound_utilities.h"

// score & record
#define INITIAL_RECORD 100
#define CHANGE_INCREMENT_THRESHOLD 100
#define LOW_INCREMENT 5
#define HIGH_INCREMENT 10

// utility variables
uint8_t new_record = 0;		// flag
uint16_t x_0_record; 
uint16_t x_f_score;
double PI;

// PONG game variables
Ball ball;
Paddle paddle;
uint32_t score = 0;
uint32_t record = INITIAL_RECORD;


void initialize_game_field (void)
{		
		// disable pause button
		disable_button(2);
	
		// compute PI
		PI = acos(-1);
		
		// clear the display
		LCD_Clear(BACKGROUND_COLOR);
				
		// initialize paddle 
		initialize_paddle(&paddle);
		
		// show score and record
		score = 0;
		display_score();
		display_record();
}

// button functions

void reset_game (void) 		// (INT0 button)
{
		uint32_t last_score;
		
		// disable the reset button
		disable_button(0);
		
		// clear old ball, if present from last game
		disable_timer(1);
		draw_centered_square(ball.x, ball.y, BALL_SIZE, BACKGROUND_COLOR);
		
		// reset score
		last_score = score;
		score = 0;
		display_new_score(last_score);
		x_f_score = 8;
		
		clear_game_over();
		
		// reset new record 
		new_record = 0;
		
		// show paddle
		show_paddle(1);
		enable_timer(1);
	
		// show start text
		GUI_Text_no_background(X_0_START, Y_0_START, (uint8_t*)START_TEXT, TEXT_COLOR);
		
		// re-draw walls and ball 
		initialize_ball(&ball);
		draw_walls();
			
		// enable start button
		enable_button(1);
}

void start_game (void)		// (KEY1 button)
{
		// clear start text
		GUI_Text_no_background(X_0_START, Y_0_START, (uint8_t*)START_TEXT, BACKGROUND_COLOR);
		
		// start moving ball & paddle
		move_ball(1);
	
		// enable pause & reset button
		enable_button(2);
		enable_button(0);
}

void pause (void)				// (KEY2 button)
{
		// write text
		GUI_Text_no_background(X_0_PAUSE, Y_0_PAUSE, (uint8_t*)PAUSE_TEXT, TEXT_COLOR);
		
		// stop ball & paddle
		disable_timer(1);
		
		play_pause_note();
}

void clear_pause(void)	// (KEY2 button)
{
		// clear text
		GUI_Text_no_background(X_0_PAUSE, Y_0_PAUSE, (uint8_t*)PAUSE_TEXT, BACKGROUND_COLOR);
		
		// move ball & paddle
		enable_timer(1);
	
		play_pause_note();
}

// score & record management

void increase_score (void)
{
		uint32_t last_score = score;
		uint32_t last_record = record;
	
		uint8_t increment = score < CHANGE_INCREMENT_THRESHOLD ? LOW_INCREMENT : HIGH_INCREMENT;
		score += increment;
		
		// display new score
		display_new_score(last_score);
	
		if(score > record)
		{	
				// update & display new record
				record = score;
				display_new_record(last_record);
				new_record = 1;
		}
}

void display_new_score (uint32_t last_score)
{
		char last_score_str[10], new_score_str[10];
		
		if(last_score == score) return;		// score did not change
		
		// convert int to string
		sprintf(last_score_str, "%d", last_score);
		sprintf(new_score_str , "%d", score);
		
		// update x_f_score
		x_f_score = X_0_SCORE + 8*strlen(new_score_str);
		
		// clear only old score text's pixels
		GUI_Text_no_background(X_0_SCORE, Y_0_SCORE, (uint8_t*)last_score_str, BACKGROUND_COLOR);
		// draw only new score text's pixels
		GUI_Text_no_background(X_0_SCORE, Y_0_SCORE, (uint8_t*)new_score_str , SCORE_COLOR);
}

void display_score (void)
{
		char score_str[10];
		
		// convert int to string
		sprintf(score_str, "%d", score);
		
		// draw only score text's pixels
		GUI_Text_no_background(X_0_SCORE, Y_0_SCORE, (uint8_t*)score_str, SCORE_COLOR);
}

void display_new_record(uint32_t last_record)
{	
		char last_record_str[10], new_record_str[10];
		uint16_t x_0_last_record, x_0_new_record;
		
		if(last_record == record) return;		// record did not change
		
		// convert int to string
		sprintf(last_record_str, "%d", last_record);
		sprintf(new_record_str , "%d", record);
		
		// compute records text's x-coordinate
		x_0_last_record = x_0_record;
		x_0_new_record  = X_f_RECORD - (8*strlen(new_record_str)) ;
		x_0_record = x_0_new_record;
	
		// clear only old record text's pixels
		GUI_Text_no_background(x_0_last_record, Y_0_RECORD, (uint8_t*)last_record_str, BACKGROUND_COLOR);
		// draw only new record text's pixels
		GUI_Text_no_background(x_0_new_record , Y_0_RECORD, (uint8_t*)new_record_str , SCORE_COLOR);
}

void display_record (void)
{
		char record_str[10];
		
		// convert int to string
		sprintf(record_str, "%d", record);
		
		// compute record text's x-coordinate
		x_0_record = X_f_RECORD - (8*strlen(record_str));
	
		// draw only record text's pixels
		GUI_Text_no_background(x_0_record, Y_0_RECORD, (uint8_t*)record_str, SCORE_COLOR);
}

// game over management

void game_over (void)
{		
		uint16_t x_0_new_record_text;
		
		// disable start & pause button
		disable_button(1);
		disable_button(2);
		
		// write lose text
		GUI_Text_no_background(X_0_GAME_OVER, Y_0_GAME_OVER, (uint8_t*)GAME_OVER_TEXT, GAME_OVER_COLOR);
		
		// write restart text
		GUI_Text_no_background(X_0_RESTART_1ST_LINE, Y_0_RESTART_1ST_LINE, (uint8_t*)RESTART_TEXT_1ST_LINE, TEXT_COLOR);
		GUI_Text_no_background(X_0_RESTART_2ND_LINE, Y_0_RESTART_2ND_LINE, (uint8_t*)RESTART_TEXT_2ND_LINE, TEXT_COLOR);
		
		// stop moving ball & paddle
		disable_timer(1);
		
		// display new record text, if necessary
		if(new_record)
		{
				// compute new record text's x-coordinate
				x_0_new_record_text = x_0_record - 8 - (8*strlen(NEW_RECORD_TEXT));
	
				// draw only new record text's pixels
				GUI_Text_no_background(x_0_new_record_text, Y_0_RECORD, (uint8_t*)NEW_RECORD_TEXT, NEW_RECORD_COLOR);
		}
}

void clear_game_over (void)
{	
		uint16_t x_0_new_record_text;
	
		// clear lose text
		GUI_Text_no_background(X_0_GAME_OVER, Y_0_GAME_OVER, (uint8_t*)GAME_OVER_TEXT, BACKGROUND_COLOR);
		
		// clear restart text
		GUI_Text_no_background(X_0_RESTART_1ST_LINE, Y_0_RESTART_1ST_LINE, (uint8_t*)RESTART_TEXT_1ST_LINE, BACKGROUND_COLOR);
		GUI_Text_no_background(X_0_RESTART_2ND_LINE, Y_0_RESTART_2ND_LINE, (uint8_t*)RESTART_TEXT_2ND_LINE, BACKGROUND_COLOR);
	
		// clear new record text, if necessary
		if(new_record)
		{		
				// compute new record text's x-coordinate
				x_0_new_record_text = x_0_record - 8 - (8*strlen(NEW_RECORD_TEXT));
	
				// clear only new record text's pixels
				GUI_Text_no_background(x_0_new_record_text, Y_0_RECORD, (uint8_t*)NEW_RECORD_TEXT, BACKGROUND_COLOR);
		}
}

