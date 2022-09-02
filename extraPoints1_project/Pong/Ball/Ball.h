#include <stdint.h>

typedef struct Ball
{		
		// (x,y) represents the middle pixel
		double last_x;
		double last_y;
		double x;			
		double y;
		// (v_x, v_y) is the actual velocity vector
		double v_x;		// [v_x] = px / 50ms
		double v_y;		// [v_y] = px / 50ms
		double v_angle;
} Ball;

// ball main functions
void initialize_ball (Ball* ball);
uint8_t change_ball_position (Ball* ball);
void draw_ball (Ball* ball);
void move_ball_square (uint16_t last_C_x, uint16_t last_C_y, uint16_t C_x, uint16_t C_y);

// ball position and hits controls
uint8_t ball_fell_down (double y);
uint8_t ball_hits_top_wall	  (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_right_wall  (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_left_wall	  (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_paddle_top  (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_paddle_left (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_paddle_right(double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_border_left (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_border_right(double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);

// ball bounces (changing position and velocity components)
void ball_bounces_horizontally(double *x, double *y, double *next_x, double reflection_x, double reflection_y, double *next_v_x, double *v_angle);
void ball_bounces_vertically	(double *x, double *y, double *next_y, double reflection_x, double reflection_y, double *next_v_y, double *v_angle);
void ball_bounces_on_paddle		(double *x, double *y, double *next_x, double *next_y, double reflection_x, double reflection_y, double *next_v_x, double *next_v_y, double *v_angle);

// read & set flags
void move_ball (uint8_t moving);
uint8_t ball_has_to_be_moved (void);
void show_ball (uint8_t show);
