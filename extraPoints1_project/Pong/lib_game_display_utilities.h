#include "../GLCD/GLCD.h"
#include <math.h>

#define BACKGROUND_COLOR Black

// wall
#define WALL_THICKNESS 5
#define WALL_COLOR Red

// Paddle
#define PADDLE_HEIGHT 10
#define PADDLE_WIDTH 	60		// to be defined
#define PADDLE_BOTTOM_DISTANCE 32		// distance (in px) from the bottom of the screen
#define PADDLE_COLOR Green

// Ball
#define BALL_SIZE	5
#define BALL_X_0 (MAX_X - 1 - WALL_THICKNESS - (BALL_SIZE/2))	// ball initial x-coordinate
#define BALL_Y_0 (MAX_Y/2)					// ball initial y-coordinate
#define BALL_V	10
#define BALL_ANGLE_0	(3.0 * PI / 4.0)	// down-left
#define BALL_COLOR Green

/* !!! IMPORTANT !!! */
#define FLUIDITY_FACTOR 4		// how many ball squares are drawn between the last position and the new one

// score & record
#define SCORE_COLOR  White
#define X_0_SCORE		10		// (X_0_SCORE, Y_0_SCORE) -> top left pixel of the score text
#define Y_0_SCORE		160
#define X_f_RECORD  (MAX_X - 10)		// (X_f_RECORD, Y_0_RECORD) -> pixel adjacent to the top right corner of the record text
#define Y_0_RECORD	10

// texts
#define TEXT_COLOR Cyan
#define START_TEXT "Press KEY1 to start :)"
#define X_0_START	((MAX_X/2) - (8*strlen(START_TEXT)/2) + 2)
#define Y_0_START	(MAX_Y/2 - 25)
#define PAUSE_TEXT "Pause"
#define X_0_PAUSE ((MAX_X/2) - (8*strlen(PAUSE_TEXT)/2))
#define Y_0_PAUSE	 (MAX_Y/2 - 25)
#define RESTART_TEXT_1ST_LINE "Press INT0 and"
#define RESTART_TEXT_2ND_LINE "KEY1 to restart"
#define X_0_RESTART_1ST_LINE	(MAX_X/2 - (8*strlen(RESTART_TEXT_1ST_LINE)/2))
#define Y_0_RESTART_1ST_LINE  (MAX_Y/2 + 25)
#define X_0_RESTART_2ND_LINE	(MAX_X/2 - (8*strlen(RESTART_TEXT_2ND_LINE)/2))
#define Y_0_RESTART_2ND_LINE  (MAX_Y/2 + 46)

// game over
#define GAME_OVER_COLOR Yellow 
#define GAME_OVER_TEXT "You lose :("
#define X_0_GAME_OVER ((MAX_X/2) - (8*strlen(GAME_OVER_TEXT)/2) + 2)
#define Y_0_GAME_OVER  (MAX_Y/2 - 25)

// new record
#define NEW_RECORD_COLOR Magenta
#define NEW_RECORD_TEXT "NEW RECORD!"


// functions
void draw_walls (void);
void draw_rectangle (uint16_t top_left_x, uint16_t top_left_y, uint16_t width, uint16_t height, uint16_t color);
void draw_rectangle_where_not_text (uint16_t top_left_x, uint16_t top_left_y, uint16_t width, uint16_t height, uint16_t color);
void draw_centered_square (uint16_t C_x, uint16_t C_y, uint16_t side, uint16_t color);
uint16_t round_double (double num);
uint16_t abs_distance (uint16_t a, uint16_t b);

uint8_t is_point_in_text_area (uint16_t x, uint16_t y);
uint8_t is_point_in_score_text_area  (uint16_t x, uint16_t y);
uint8_t is_point_in_record_text_area (uint16_t x, uint16_t y);

