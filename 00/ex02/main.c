#include <avr/io.h>

int main() {
	DDRB |= (1 << PB0);
	PORTB &= ~(1 << PB0);

	while (1) {
		// PIND is input read 
		if (!(PIND & (1 << PD2))) {
			PORTB |= (1 << PB0);
		}
		else {
			PORTB &= ~(1 << PB0);
		}
	}
}
