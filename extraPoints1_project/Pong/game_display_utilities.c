#include "lib_game_display_utilities.h"
#include <math.h>

extern uint16_t x_0_record;
extern uint16_t x_f_score;

void draw_rectangle (uint16_t top_left_x, uint16_t top_left_y, uint16_t width, uint16_t height, uint16_t color)
{
		uint16_t i,j;
	
		if(width == 0 || height == 0) return; 	// no area
		
		if(top_left_x >= MAX_X || top_left_y >= MAX_Y) return;	// outcale -> don't drow it
	
		// if the rectangle does not fit the screen, draw only the part inside the screen
		if(top_left_x + width > MAX_X)
				width = MAX_X - top_left_x;
		
		if(top_left_y + height > MAX_Y)
				height = MAX_Y - top_left_y;
	
		for(j=0; j<height; j++)
				for(i=0; i<width; i++)
						LCD_SetPoint(top_left_x + i, top_left_y + j, color);
}

void draw_rectangle_where_not_text (uint16_t top_left_x, uint16_t top_left_y, uint16_t width, uint16_t height, uint16_t color)
{
		uint16_t i,j;
		volatile uint16_t curr_color;
	
		if(width == 0 || height == 0) return; 	// no area
		
		if(top_left_x >= MAX_X || top_left_y >= MAX_Y) return;	// outcale -> don't drow it
	
		// if the rectangle does not fit the screen, draw only the part inside the screen
		if(top_left_x + width > MAX_X)
				width = MAX_X - top_left_x;
		
		if(top_left_y + height > MAX_Y)
				height = MAX_Y - top_left_y;
	
		for(j=0; j<height; j++)
		{
				for(i=0; i<width; i++)
				{	
						if(is_point_in_text_area(top_left_x + i, top_left_y + j))
						{
								curr_color = LCD_GetPoint(top_left_x + i, top_left_y + j);
					
								if(curr_color != SCORE_COLOR)		//if it is not a text's pixel
										LCD_SetPoint(top_left_x + i, top_left_y + j, color);
						}
						else
						{
								LCD_SetPoint(top_left_x + i, top_left_y + j, color);
						}
				}
		}
}

// draw a square knowing its center pixels' coordinates (C_x, C_y)
void draw_centered_square (uint16_t C_x, uint16_t C_y, uint16_t side, uint16_t color)
{
		//draw a square
		draw_rectangle(C_x-(side/2), C_y-(side/2), side, side, color);
}

void draw_walls (void)
{		
		uint16_t	side_walls_height = MAX_Y - PADDLE_BOTTOM_DISTANCE - PADDLE_HEIGHT - WALL_THICKNESS ;
	
		// top wall
		draw_rectangle(0, 0, MAX_X, WALL_THICKNESS, WALL_COLOR);
		
		// left wall
		draw_rectangle(0, WALL_THICKNESS, WALL_THICKNESS, side_walls_height, WALL_COLOR);
		
		// right wall
		draw_rectangle(MAX_X-WALL_THICKNESS, WALL_THICKNESS, WALL_THICKNESS, side_walls_height, WALL_COLOR);
}

// it computes the floor(x) function of a double and return a uint16_t integer
uint16_t round_double (double num)
{	
		return num >= 0 ? (uint16_t)num : 0;
}

// it computes the absolute difference between two uint16_t integers
uint16_t abs_distance (uint16_t a, uint16_t b)
{
		return a > b ? (a-b) : (b-a);
}

// boolean function that says if (x,y) pixel is in the score text area 
uint8_t is_point_in_text_area (uint16_t x, uint16_t y)
{
		return  is_point_in_score_text_area(x,y) || is_point_in_record_text_area(x,y);
}

// boolean function that says if (x,y) pixel is in the score text area 
uint8_t is_point_in_score_text_area (uint16_t x, uint16_t y)
{
		return  ((x >= X_0_SCORE ) && (x <  x_f_score)  &&			// (x,y) is in score text's area
					   (y >= Y_0_SCORE ) && (y <  Y_0_SCORE  + 16));
}

// boolean function that says if (x,y) pixel is in the record text area 
uint8_t is_point_in_record_text_area (uint16_t x, uint16_t y)
{
		return 	((x >= x_0_record) && (x <  X_f_RECORD) &&	  	// (x,y) is in record text's area
					   (y >= Y_0_RECORD) && (y <  Y_0_RECORD + 16));
}

