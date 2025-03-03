#include <avr/io.h>
#include <util/delay.h>

int on_released(uint8_t P_ID) {
	volatile uint8_t  i = 0;

	if (!(PIND & (1 << P_ID))) {
		//delay for avoid bonce effect
		_delay_ms(20);
		i++;
	}
	if ((PIND & (1 << P_ID))) {
		i++;
	}

	if (i == 2) {
		return 1;
	}
	return 0;
}

int main() {
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
	volatile uint8_t  value = 0;

	while (1) {

		volatile uint8_t  i = 0;


		if (on_released(PD2)) {
			value++;
		}
		if (on_released(PD4)){
			value--;
		}

		PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
		PORTB |= ((value & (1 << 0)));
		PORTB |= ((value & (1 << 1)));
		PORTB |= ((value & (1 << 2)));
		PORTB |= ((value & (1 << 3)) << 1);

	}
}
