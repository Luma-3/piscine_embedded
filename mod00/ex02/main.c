#include <avr/io.h>

int main() {
	DDRB |= (1 << PB0);
	PORTB &= ~(1 << PB0);

	while (1) {
		// PINX is input read only register, D for secteur and PIND2 for selec correct pin to read
		if (!(PIND & (1 << PIND2))) {
			PORTB |= (1 << PB0);
		}
		else {
			PORTB &= ~(1 << PB0);
		}
	}
}
