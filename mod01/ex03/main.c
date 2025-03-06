#include <avr/io.h>
#include <util/delay.h>

#define CLOCK F_CPU / 1024
#define EPSILON 0.01

struct button
{
	uint8_t pin;
	uint8_t last_state;
};

int on_pressed(struct button *button) {
	uint8_t current_state = (PIND & (1 << button->pin)) ? 1 : 0;
	_delay_ms(20);
	uint8_t result = (button->last_state == 1 && current_state == 0);
	button->last_state = current_state;
	return result;
}



int main() {
	DDRB |= (1 << PB1) | (1 << PB0);
	float value = 0.1;

	// COM1A1 for clean OC1A on match

	// active WGM11 & WGM 12 & WGM13 for fast PWM mode (page 109 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
	TCCR1A |=  (1 << COM1A1) | (1 << WGM11);
	TCCR1B |=  (1 << CS12) | (1 << CS10) | (1 << WGM12) | (1 << WGM13);
	
	//OCR1A is limite for down flag (because COM1A1 is active) see (page 108 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf) 
	OCR1A = (uint16_t)CLOCK * value;
	// ICR1 is limite of periode (TOP) 
	ICR1 = CLOCK ;

	struct button b1 = {PIND2, 1};
	struct button b2 = {PIND4, 1};


	while (1) {

		if (on_pressed(&b1)) {
			if (value < 1) {
				value += 0.1;
			}
		}

		if (on_pressed(&b2)) {
			//0.11 , EPSILON is add for adjust float calculating inaccuracy
			if (value > 0.1 + EPSILON) {
				value -= 0.1;
			}
		}
		OCR1A = CLOCK * value;
	}
}