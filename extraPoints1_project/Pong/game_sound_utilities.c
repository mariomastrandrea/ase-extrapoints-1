#include "lib_game_sound_utilities.h"
#include "../timer/timer.h"

#ifdef SIMULATOR
	#define SCALING_FACTOR	35		// the more similar it is to the simulator scaling factor the more real are the played notes

	// I multiply k values by an approximation of the scaling factor in order to
	// reduce the emulator's TIMER scaling effect and try to play more real notes
	#define PAUSE_NOTE_FREQUENCY_K     ( 398*SCALING_FACTOR)    // k=398  -> f6 (1396Hz)
	#define HIGH_NOTE_FREQUENCY_K      ( 796*SCALING_FACTOR)    // k=796  -> f5 (698Hz)
	#define LOW_NOTE_FREQUENCY_K 	     (1062*SCALING_FACTOR) 	  // k=1062 -> c5 (523Hz)
	#define GAME_OVER_NOTE_FREQUENCY_K (2124*SCALING_FACTOR)    // k=2124 -> c4 (262Hz)

#else
	#define PAUSE_NOTE_FREQUENCY_K       398	   // k=398  -> f6 (1396Hz)
	#define HIGH_NOTE_FREQUENCY_K        796	   // k=796  -> f5 (698Hz)
	#define LOW_NOTE_FREQUENCY_K 	      1062 	   // k=1062 -> c5 (523Hz)
	#define GAME_OVER_NOTE_FREQUENCY_K  2124     // k=2124 -> c4 (262Hz)

#endif

extern int ticks;

void play_higher_pitched_note (void)
{
		disable_timer(2);
		reset_timer(2);
	
		init_timer(2, HIGH_NOTE_FREQUENCY_K, 0);
		ticks = 0;
		enable_timer(2); 
}

void play_lower_pitched_note (void)
{
		disable_timer(2);
		reset_timer(2);
		
		init_timer(2, LOW_NOTE_FREQUENCY_K, 0);
		ticks = 0;
		enable_timer(2);
}

void play_game_over_note (void)
{
		disable_timer(2);
		reset_timer(2);
		
		init_timer(2, GAME_OVER_NOTE_FREQUENCY_K, 0);
		ticks = 0;
		enable_timer(2);
}

void play_pause_note (void)
{
		disable_timer(2);
		reset_timer(2);
		
		init_timer(2, PAUSE_NOTE_FREQUENCY_K, 0);
		ticks = 0;
		enable_timer(2);
}
