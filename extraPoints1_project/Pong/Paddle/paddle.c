#include "paddle.h"
#include "../lib_game_display_utilities.h"

uint8_t to_show_paddle;
uint8_t showed_paddle;

void initialize_paddle(Paddle* paddle)
{
		// setting an outscale value for position 
		paddle->last_x_position = MAX_X;
		paddle->x_position 			= MAX_X;
		paddle->next_x_position = MAX_X;
	
		paddle->y_position = MAX_Y - PADDLE_BOTTOM_DISTANCE - PADDLE_HEIGHT;
		
		to_show_paddle = 0;
		showed_paddle = 0;
}

// change paddle position by means of the current x_value and the previous ones
uint8_t set_paddle_position (Paddle* paddle, uint16_t x_value, uint16_t x_scale)
{
		uint16_t paddle_max_x = MAX_X - PADDLE_WIDTH;
		uint16_t new_position = x_value * paddle_max_x / x_scale;
		
		// ****************************************************
		// * To handle potmeter non ideality: 								*
		// *	1) consider only changes by >=10 px;						*
		// *	2) change paddle position only if it maintains 	*
		// *		 roughly the same value for 2 cycles					*
		// ****************************************************
	
		if(abs_distance(new_position, paddle->next_x_position) < 10 
				&& new_position != 0 && new_position != paddle_max_x)	// in order to avoid problems at positions 0 and paddle_max_x, always consider this two values 'new' 
		{
					// position not changed significantly since last time
				
					if(paddle->next_x_position == paddle->x_position)	
						return 0;		// no changes for a long time -> nothing changes
					
					// position changed only for 1 cycle -> update positions and show new paddle image
					paddle->last_x_position = paddle->x_position;
					paddle->x_position = paddle->next_x_position;
					// (paddle->next_x_position remains the same, so it is equal to paddle->x_position)
					return 1;	// show new paddle
		}
				
		// position changed since last time
		
		// update positions only if paddle has just been rendered, in order to save its position in last_x_position
		if(paddle->x_position == paddle->next_x_position)
		{
				paddle->last_x_position = paddle->x_position; 
				paddle->x_position = paddle->next_x_position;
				paddle->next_x_position = new_position;
		}
		else if (abs_distance(new_position, paddle->x_position) >= 10)
		// if measured position changed too frequently, update only next x-position, but only if significantly different from current position
				paddle->next_x_position = new_position;	
		
		return 0;		
}

void draw_paddle (Paddle* paddle)
{
		uint16_t next_position, last_position, distance, i;
		
		if(!showed_paddle)	// paddle has to be drawn for the first time
		{
				draw_rectangle(paddle->next_x_position, paddle->y_position, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_COLOR);
				showed_paddle = 1;
				return;
		}
		
		next_position = paddle->x_position;
		last_position = paddle->last_x_position;
			
		distance = abs_distance(next_position, last_position);
	
		if(distance == 0) return;		// no changes in position
		
		// paddle position changed here
	
		if(next_position > last_position) // => paddle moved to the right
		{
				// move paddle to the right one pixel at a time
				for(i=0; i<distance; i++)
				{
						// erase an old vertical line
						draw_rectangle(last_position + i, paddle->y_position, 1, PADDLE_HEIGHT, BACKGROUND_COLOR);
						// draw a new vertical line
						draw_rectangle(last_position + PADDLE_WIDTH + i, paddle->y_position, 1, PADDLE_HEIGHT, PADDLE_COLOR);
				}
		}
		else // <= paddle moved to the left
		{
				// move paddle to the left one pixel at a time
				for(i=0; i<distance; i++)
				{
						// erase an old vertical line
						draw_rectangle(last_position + PADDLE_WIDTH - i - 1, paddle->y_position, 1, PADDLE_HEIGHT, BACKGROUND_COLOR);
						// draw a new vertical line
						draw_rectangle(last_position - i - 1, paddle->y_position, 1, PADDLE_HEIGHT, PADDLE_COLOR);
				}
		}
}

// read and set flags

uint8_t paddle_has_to_be_shown (void)
{
		return to_show_paddle;
}

void show_paddle (uint8_t show)
{
		to_show_paddle = show;
}
