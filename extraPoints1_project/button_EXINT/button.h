#include <stdint.h>

void BUTTON_init(void);

void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

void enable_button (uint8_t num_button);
void disable_button(uint8_t num_button);

