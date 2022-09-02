#include <stdint.h>

void initialize_game_field (void);
void reset_game (void);		// (INT0 button)
void start_game (void);		// (KEY1 button)
void pause (void);				// (KEY2 button)
void clear_pause(void);

void increase_score (void);		// increase and display new score; in case of new record, update and display it
void display_new_score (uint32_t last_score);
void display_score (void);
void display_new_record (uint32_t last_record);
void display_record (void);

void game_over (void);
void clear_game_over (void);


