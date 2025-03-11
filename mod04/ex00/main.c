#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define _SEI() {SREG |= (1<< SREG_I);}

volatile uint8_t flag = 0;

ISR(INT0_vect) {
	flag = 1;
}

int main() {
	DDRB |= (1 << PB0);

	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);
	_SEI();

	while (1) {
		if (flag == 1){
			PORTB ^= (1 << PB0);
			_delay_ms(200);
			flag = 0;
		}
	}
}