#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRB |= (1 << PB0);
	PORTB &= ~(1 << PB0);
	volatile uint8_t  i = 0;

	while (1) {
		i = 0;
		// double if with value for release type input button
		if (!(PIND & (1 << PD2))) {
			//delay for avoid bonce effect 
			_delay_ms(20);
			i++;
		}
		if ((PIND & (1 << PD2))) {
			i++;
		}

		// if press and release this condition is true and inverse toggle the led
		if (i == 2) {
			PORTB ^= (1 << PB0);
		}
	}
}
