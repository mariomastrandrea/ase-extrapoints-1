#include "Ball.h"
#include "../lib_game_display_utilities.h"
#include "../Paddle/paddle.h"
#include "../lib_game_sound_utilities.h"
#include "../../timer/timer.h"
#include "../pong_model.h"

#define MIN_REFLECTION_ANGLE (17.0 * PI / 16.0)
#define MAX_REFLECTION_ANGLE (31.0 * PI / 16.0)
extern double PI;

uint8_t moving_ball = 0;
uint8_t showed_ball = 0;

double ball_min_x = WALL_THICKNESS + (int)(BALL_SIZE/2);
double ball_max_x = MAX_X - WALL_THICKNESS - ((int)(BALL_SIZE/2));
double ball_min_y = WALL_THICKNESS + (int)(BALL_SIZE/2);
double ball_max_y = MAX_Y - PADDLE_BOTTOM_DISTANCE - PADDLE_HEIGHT - ((int)(BALL_SIZE/2));

double wall_max_y = MAX_Y - PADDLE_BOTTOM_DISTANCE - PADDLE_HEIGHT;

double border_left_x  = (int)(BALL_SIZE/2);
double border_right_x = MAX_X - ((int)(BALL_SIZE/2));

extern Paddle paddle;
double paddle_min_x;
double paddle_max_x;


void initialize_ball (Ball* ball)
{	
		ball->last_x = (double)(MAX_X + BALL_SIZE);		// outscale value
		ball->last_y = (double)(MAX_Y + BALL_SIZE);		// outscale value
		ball->x 		 = (double) BALL_X_0;
		ball->y 		 = (double) BALL_Y_0;

		ball->v_x = BALL_V * cos(BALL_ANGLE_0);
		ball->v_y = BALL_V * sin(BALL_ANGLE_0);
		ball->v_angle = BALL_ANGLE_0;
	
		moving_ball = 0;
		showed_ball = 0;
		draw_ball(ball);
}

uint8_t change_ball_position (Ball* ball)
{
		double x, y, next_x, next_y, next_v_x, next_v_y, hit_x, hit_y;
		
		// compute next (x,y) position components
		next_x = ball->x + ball->v_x;
		next_y = ball->y + ball->v_y;
		
		// save temporary values
		x = ball->x;
		y = ball->y;
		next_v_x = ball->v_x;
		next_v_y = ball->v_y;
		
		if(ball_fell_down(y))
		{	
				game_over();
				play_game_over_note();
				return 0;
		}
		
		// (multiple) hits controls 
	
		if(ball_hits_top_wall(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_vertically(&x, &y, &next_y, hit_x, hit_y, &next_v_y, &(ball->v_angle));
				play_lower_pitched_note();
				
				if(ball_hits_left_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
						play_lower_pitched_note();
				}
				else if(ball_hits_right_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
						play_lower_pitched_note();
				}
		}
		else if(ball_hits_left_wall(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				play_lower_pitched_note();
			
				if(ball_hits_top_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_vertically(&x, &y, &next_y, hit_x, hit_y, &next_v_y, &(ball->v_angle));
						play_lower_pitched_note();
				}
				else if(ball_hits_paddle_top(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_on_paddle(&x, &y, &next_x, &next_y, hit_x, hit_y, &next_v_x, &next_v_y, &(ball->v_angle)); 
						play_higher_pitched_note();
						increase_score();
				}
		}
		else if(ball_hits_right_wall(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				play_lower_pitched_note();
			
				if(ball_hits_top_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_vertically(&x, &y, &next_y, hit_x, hit_y, &next_v_y, &(ball->v_angle));
						play_lower_pitched_note();
				}
				else if(ball_hits_paddle_top(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_on_paddle(&x, &y, &next_x, &next_y, hit_x, hit_y, &next_v_x, &next_v_y, &(ball->v_angle)); 
						play_higher_pitched_note();
						increase_score();
				}
		}
		else if(ball_hits_paddle_top(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_on_paddle(&x, &y, &next_x, &next_y, hit_x, hit_y, &next_v_x, &next_v_y, &(ball->v_angle)); 
				play_higher_pitched_note();
				increase_score();
			
				if(ball_hits_left_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
						play_lower_pitched_note();
				}
				else if(ball_hits_right_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
						play_lower_pitched_note();
				}
		}
		else if(ball_hits_paddle_left(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				play_higher_pitched_note();
			
				if(ball_hits_left_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
						play_lower_pitched_note();
				}
				else if(ball_hits_border_left(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				}
		}
		else if(ball_hits_paddle_right(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				play_higher_pitched_note();
			
				if(ball_hits_right_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
						play_lower_pitched_note();
				}
				else if(ball_hits_border_right(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				}
		}
		else if(ball_hits_border_right(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				
				if(ball_hits_paddle_right(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
						play_higher_pitched_note();
				}
		}
		else if(ball_hits_border_left(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
							
				if(ball_hits_paddle_left(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
						play_higher_pitched_note();
				}
		}
		
		// ball info updates
		
		ball->last_x = ball->x;
		ball->last_y = ball->y;
		ball->x = next_x;
		ball->y = next_y;
		ball->v_x = next_v_x;
		ball->v_y = next_v_y;
		
		return (ball->last_x != ball->x || ball->last_y != ball->y);
}

void draw_ball (Ball* ball)
{
		uint16_t C_x, C_y, i;
		double t = 0;
		double last_temp_x, last_temp_y, temp_x, temp_y;
		
		if(!showed_ball) 	// for the first time, draw the entire square on the screen
		{
				// compute the pixel closest to the actual ball center
				C_x = round_double(ball->x);	
				C_y = round_double(ball->y);
				
				draw_centered_square (C_x, C_y, BALL_SIZE, BALL_COLOR);
				showed_ball = 1;
				return;
		}
		
		last_temp_x = ball->last_x;
		last_temp_y = ball->last_y;
		
		// draw a sequence of pixels from last position to the new one
		for(i=1; i<=FLUIDITY_FACTOR; i++)
		{
				t = (double)i / (double)FLUIDITY_FACTOR;

				temp_x = ball->last_x + t*(ball->x - ball->last_x);
				temp_y = ball->last_y + t*(ball->y - ball->last_y);
				
				move_ball_square(round_double(last_temp_x), round_double(last_temp_y), round_double(temp_x), round_double(temp_y));
				
				last_temp_x = temp_x;
				last_temp_y = temp_y;
		}
			
		// re-draw the score (or the record) if the ball went through the score's (or the record's) text area
		
		if(is_point_in_score_text_area(ball->last_x, ball->last_y))
				display_score();
		else if (is_point_in_record_text_area(ball->last_x, ball->last_y))
				display_record();
}

// move a square image from the old position (last_C_x, last_C_y) to the new one (C_x, C_y)
void move_ball_square (uint16_t last_C_x, uint16_t last_C_y, uint16_t C_x, uint16_t C_y) 
{
		uint16_t A_x, A_y, B_x, B_y;
		int distance_x, distance_y;
	
		if(C_x == last_C_x && C_y == last_C_y) return;	// no changes respect to the last image
		
		if(abs_distance(C_x, last_C_x) >= BALL_SIZE || 
			 abs_distance(C_y, last_C_y) >= BALL_SIZE)		// no overlap between last ball image and the new one
		{
				// erase entire old image
				draw_centered_square (last_C_x, last_C_y, BALL_SIZE, BACKGROUND_COLOR);
				// draw entire new image
				draw_centered_square (C_x, C_y, BALL_SIZE, BALL_COLOR);
				return;
		}
		
		// there is overlap -> erase only the right pixels and draw only the new ones
		
		distance_x = C_x - last_C_x;
		distance_y = C_y - last_C_y;
		
		// A -> top-left corner of last image
		A_x = last_C_x - (int)(BALL_SIZE/2);
		A_y = last_C_y - (int)(BALL_SIZE/2);
		// B -> top-left corner of new image
		B_x = C_x - (int)(BALL_SIZE/2);
		B_y = C_y - (int)(BALL_SIZE/2);
			
		if(distance_x >= 0 && distance_y >= 0)		// movement down to the right
		{		
				// erase old parts
				draw_rectangle(A_x, A_y, BALL_SIZE, (uint16_t)distance_y, BACKGROUND_COLOR);
				draw_rectangle(A_x, A_y + distance_y, distance_x, BALL_SIZE - distance_y, BACKGROUND_COLOR);
				// draw new parts
				draw_rectangle(B_x + BALL_SIZE - distance_x, B_y, distance_x, BALL_SIZE - distance_y, BALL_COLOR);
				draw_rectangle(B_x, B_y + BALL_SIZE - distance_y, BALL_SIZE, distance_y, BALL_COLOR);
		}
		else if (distance_x <= 0 && distance_y <= 0)	// movement up to the left
		{
				// erase old parts
				draw_rectangle(A_x + (uint16_t)(BALL_SIZE + distance_x), A_y, -distance_x, BALL_SIZE + distance_y, BACKGROUND_COLOR);
				draw_rectangle(A_x, A_y + (uint16_t)(BALL_SIZE + distance_y), BALL_SIZE, -distance_y, BACKGROUND_COLOR);
				// draw new parts
				draw_rectangle(B_x, B_y, BALL_SIZE, -distance_y, BALL_COLOR);
				draw_rectangle(B_x, B_y + (uint16_t)(-distance_y), -distance_x, BALL_SIZE + distance_y, BALL_COLOR);
		}
		else if (distance_x > 0 && distance_y < 0) 	// movement up to the right
		{
				// erase old parts
				draw_rectangle(A_x, A_y, distance_x, BALL_SIZE + distance_y, BACKGROUND_COLOR);
				draw_rectangle(A_x, A_y + (uint16_t)(BALL_SIZE + distance_y), BALL_SIZE, -distance_y, BACKGROUND_COLOR);
				// draw new parts
				draw_rectangle(B_x, B_y, BALL_SIZE, -distance_y, BALL_COLOR);
				draw_rectangle(B_x + (uint16_t)(BALL_SIZE - distance_x), B_y + (uint16_t)(-distance_y), distance_x, BALL_SIZE + distance_y, BALL_COLOR);
		}
		else if (distance_x < 0 && distance_y > 0)	 // movement down to the left
		{
				// erase old parts
				draw_rectangle(A_x, A_y, BALL_SIZE, distance_y, BACKGROUND_COLOR);
				draw_rectangle(A_x + (uint16_t)(BALL_SIZE + distance_x), A_y + distance_y, -distance_x, BALL_SIZE - distance_y, BACKGROUND_COLOR);
				// draw new parts
				draw_rectangle(B_x, B_y, -distance_x, BALL_SIZE - distance_y, BALL_COLOR);
				draw_rectangle(B_x, B_y + (uint16_t)(BALL_SIZE - distance_y), BALL_SIZE, distance_y, BALL_COLOR);
		}
}

// hits controls

// check if top wall is going to be hit by the ball
uint8_t ball_hits_top_wall(double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_x;
			
		if(next_y >= ball_min_y) return 0;		// no top wall hit
		
		if(y < ball_min_y) return 0;	// ball was above the top wall yet
		
		// next_y above the top wall -> compute the touch point 
		touch_x = x + ((ball_min_y - y)/(next_y - y))*(next_x - x);
		
		if(touch_x >= ball_min_x && touch_x <= ball_max_x) 
		{
				*hit_x = touch_x;
				*hit_y = ball_min_y;
				return 1;	// top wall was hit
		}
		
		return 0;		// maybe another element was hit
}

// check if right wall is going to be hit by the ball
uint8_t ball_hits_right_wall(double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y;
		
		if(next_x < ball_max_x) return 0;		// no right wall hit
		
		if(x >= ball_max_x) return 0; 		// ball was beyond the right wall x yet
				
		// ball is going beyond the right wall -> compute the touch point
		touch_y = y + ((ball_max_x - x)/(next_x - x))*(next_y - y);
		
		if(touch_y >= ball_min_y && touch_y <= wall_max_y)
		{
				*hit_x = ball_max_x;
				*hit_y = touch_y;
				return 1;	// right wall was hit
		}
		
		return 0;		// maybe another element was hit
}

// check if left wall is going to be hit by the ball
uint8_t ball_hits_left_wall(double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y;
		
		if(next_x >= ball_min_x) return 0;		// no left wall hit
		
		if(x < ball_min_x) return 0;	// ball was beyond the left wall x yet
			
		// ball is going beyond the left wall -> compute the touch point
		touch_y = y + ((ball_min_x - x)/(next_x - x))*(next_y - y);
		
		if(touch_y >= ball_min_y && touch_y <= wall_max_y)
		{
				*hit_x = ball_min_x;
				*hit_y = touch_y;
				return 1;	// left wall was hit
		}
		
		return 0;		// maybe another element was hit
}

// check if paddle top is going to be hit by the ball
uint8_t ball_hits_paddle_top(double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_x;
		
		if(next_y < ball_max_y) return 0; 	// no paddle top hit
		
		if(y >= ball_max_y) return 0;	// ball was below paddle yet
		
		// ball is going below the top paddle -> compute the touch point
		touch_x = x + ((ball_max_y - y)/(next_y - y))*(next_x - x);
		
		paddle_min_x = (double)(paddle.x_position) - ((int)(BALL_SIZE/2) + 1);
		paddle_max_x = (double)(paddle.x_position) + PADDLE_WIDTH + ((int)(BALL_SIZE/2) + 1);
		
		if(touch_x >= paddle_min_x && touch_x <= paddle_max_x)
		{
				*hit_x = touch_x;
				*hit_y = ball_max_y;
				return 1;	// paddle top was hit
		}
		
		return 0;		// maybe another element was hit
}

// check if paddle right is going to be hit by the ball
uint8_t ball_hits_paddle_right (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y, paddle_min_y, paddle_max_y;
		
		double paddle_right_x = paddle.x_position + PADDLE_WIDTH + (int)(BALL_SIZE/2);
		
		if(next_x >= paddle_right_x) return 0; // no paddle right hit
		
		if(x < paddle_right_x) return 0;	// ball was beyond paddle right yet
		
		// ball is going to pass through the right paddle surface -> compute the touch point
		touch_y = y + ((paddle_right_x - x)/(next_x - x))*(next_y - y);
			
		paddle_min_y = ball_max_y;
		paddle_max_y = MAX_Y - PADDLE_BOTTOM_DISTANCE + (int)(BALL_SIZE/2);
	
		if(touch_y > paddle_min_y && touch_y < paddle_max_y) 
		{
				*hit_x = paddle_right_x;
				*hit_y = touch_y;
				return 1;		// paddle right was hit
		}
		
		return 0; 	// maybe another element was hit
}

// check if paddle left is going to be hit by the ball
uint8_t ball_hits_paddle_left (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y, paddle_min_y, paddle_max_y;
		
		double paddle_left_x = paddle.x_position - ((int)(BALL_SIZE/2) + 1);
		
		if(next_x < paddle_left_x) return 0; // no paddle left hit
		
		if(x >= paddle_left_x) return 0;	// ball was beyond paddle left yet
		
		// ball is going to pass through the left paddle surface -> compute the touch point
		touch_y = y + ((paddle_left_x - x)/(next_x - x))*(next_y - y);
			
		paddle_min_y = ball_max_y;
		paddle_max_y = MAX_Y - PADDLE_BOTTOM_DISTANCE + (int)(BALL_SIZE/2);
	
		if(touch_y > paddle_min_y && touch_y < paddle_max_y) 
		{
				*hit_x = paddle_left_x;
				*hit_y = touch_y;
				return 1;		// paddle left was hit
		}
		
		return 0; 	// maybe another element was hit
}

// check if border left is going to be hit by the ball
uint8_t ball_hits_border_left (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y;
		if(next_x >= border_left_x) return 0;		// no border left hit
		
		if(x == next_x) return 0; 	// to avoid division by 0
		
		// ball is going beyond the left border -> compute the touch point
		touch_y = y + ((border_left_x - x)/(next_x - x))*(next_y - y);
		
		if(touch_y >= MAX_Y - PADDLE_BOTTOM_DISTANCE - PADDLE_HEIGHT + ((int)(BALL_SIZE/2)))
		{
				*hit_x = border_left_x;
				*hit_y = touch_y;
				return 1;		// border left was hit
		}
		
		return 0;		// maybe another element was hit
}

// check if border right is going to be hit by the ball
uint8_t ball_hits_border_right (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y;
		if(next_x < border_right_x) return 0;		// no border right hit
		
		if(x == next_x) return 0; 	// to avoid division by 0
		
		// ball is going beyond the right border -> compute the touch point
		touch_y = y + ((border_right_x - x)/(next_x - x))*(next_y - y);
		
		if(touch_y >= MAX_Y - PADDLE_BOTTOM_DISTANCE - PADDLE_HEIGHT + ((int)(BALL_SIZE/2)))
		{
				*hit_x = border_right_x;
				*hit_y = touch_y;
				return 1;		// border right was hit
		}
		
		return 0;		// maybe another element was hit
}

// check if ball fell under the screen
uint8_t ball_fell_down (double y)
{
		return y >= MAX_Y + BALL_SIZE;
}

// ball bouncing functions below

void ball_bounces_horizontally (double *x, double *y, double *next_x, double reflection_x, double reflection_y, double *next_v_x, double *v_angle)
{
		double x_to_reflect = *next_x;
		double v_x_to_reflect = *next_v_x;
	
		// update origin coordinates
		*x = reflection_x;
		*y = reflection_y;
		
		// apply reflection respect to vertical axis
		*next_x = 2*reflection_x - x_to_reflect;		// new x-coordinate
		*next_v_x = -v_x_to_reflect;								// new v_x
		*v_angle = (PI - *v_angle) > 0 ? (PI - *v_angle) : (3.0 * PI - *v_angle);
}

void ball_bounces_vertically (double *x, double *y, double *next_y, double reflection_x, double reflection_y, double *next_v_y, double *v_angle)
{
		double y_to_reflect = *next_y;
		double v_y_to_reflect = *next_v_y;
	
		// update origin coordinates
		*x = reflection_x;
		*y = reflection_y;
		
		// apply reflection respect to horizontal axis
		*next_y = 2*reflection_y - y_to_reflect;		// new y-coordinate
		*next_v_y = -v_y_to_reflect;								// new v_y
		*v_angle = 2.0*PI - *v_angle; 							// new velocity angle
}

void ball_bounces_on_paddle (double *x, double *y, double *next_x, double *next_y, double reflection_x, 
														  double reflection_y, double *next_v_x, double *next_v_y, double *v_angle)
{
		double old_v_angle, new_v_angle, rotation_angle;
	
		// update origin coordinates
		*x = reflection_x;
		*y = reflection_y;
		
		// compute new velocity angle
		// the new angle depends linearly on the paddle point touched by the ball
		new_v_angle = MIN_REFLECTION_ANGLE + ((reflection_x - paddle_min_x)/(paddle_max_x - paddle_min_x))*(MAX_REFLECTION_ANGLE - MIN_REFLECTION_ANGLE);
		
		// compute the new velocity coordinates (v_x, v_y) maintaining the same velocity module (BALL_V)
		*next_v_x = BALL_V * cos(new_v_angle);
		*next_v_y = BALL_V * sin(new_v_angle);
	
		// computing rotation angle 
		old_v_angle = *v_angle;
		rotation_angle = new_v_angle - old_v_angle;
		
		// computing new (x,y) coordinates applying a rotation over the paddle touched point
		*next_x = (*next_x - reflection_x)*cos(rotation_angle) - (*next_y - reflection_y)*sin(rotation_angle) + reflection_x;
		*next_y = (*next_x - reflection_x)*sin(rotation_angle) + (*next_y - reflection_y)*cos(rotation_angle) + reflection_y;
		
		// update v angle
		*v_angle = new_v_angle;
}

// read & set flags

void move_ball (uint8_t moving)
{
		moving_ball = moving;
}

uint8_t ball_has_to_be_moved (void)
{
		return moving_ball;
}

void show_ball (uint8_t show)
{
		showed_ball = show;
}

