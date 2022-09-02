#include <stdint.h>

// NOTE: (x,y) are the coordinates of the top left corner
//	 			So x ranges from 0 to (MAX_X - PADDLE_WIDTH)

typedef struct Paddle
{
		// save last, current and next position, in order to handling potmeter non ideality
	  // -> paddle will move only if next_position is similar to current_position and they are different from the last one
		uint16_t last_x_position;
		uint16_t x_position;
		uint16_t next_x_position;
	
		uint16_t y_position; 	
} Paddle;

// paddle main functions
void initialize_paddle (Paddle* paddle);
uint8_t set_paddle_position (Paddle* paddle, uint16_t x_value, uint16_t x_scale);
void draw_paddle (Paddle* paddle);

// read & set flags
uint8_t paddle_has_to_be_shown (void);
void show_paddle (uint8_t show);
