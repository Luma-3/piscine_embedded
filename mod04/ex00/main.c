#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT0_vect) {
	_delay_ms(50);
	PORTB ^= (1 << PB0);
}

int main() {
	DDRB |= (1 << PB0);

	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);
	sei();

	while (1);
}