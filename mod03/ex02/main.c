#include <avr/io.h>
#include <util/delay.h>

#define R (1 << PD5)
#define G (1 << PD6)
#define B (1 << PD3)

#define RGB_LED(A) {			\
	PORTD ^= A;					\
	_delay_ms(1000);			\
	PORTD ^= A;					\
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	OCR0A = g;
	OCR0B = r;
	OCR2B = b;
}

void init_rgb() {
	DDRD |= R | G | B;
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1); // Fast PWM Clear OCR0A OCR0B
	TCCR0B |= (1 << CS00); // Active  Timer divid 1
	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1); // Fast PWM  Clear OCR2B
	TCCR2B |= (1 << CS20);// Active  Timer divid 1
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

int main() {
	init_rgb();

	uint8_t i = 0;
	while (1) {
		wheel(i++);
		_delay_ms(30);
	}
}
