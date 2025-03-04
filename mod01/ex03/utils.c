#include <avr/io.h>


struct button
{
	uint8_t pin;
	uint8_t last_state;
};

int on_pressed(struct button *button) {
	uint8_t current_state = (PIND & (1 << button->last_state));
	uint8_t result = (button->last_state == 1 && current_state == 0);
	button->last_state = current_state;
	return result;
}