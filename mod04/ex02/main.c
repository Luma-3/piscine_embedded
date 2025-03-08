#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void refresh(uint8_t value) {
	PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
	PORTB |= ((value & (1 << 0)));
	PORTB |= ((value & (1 << 1)));
	PORTB |= ((value & (1 << 2)));
	PORTB |= ((value & (1 << 3)) << 1); // shift one for correct pin 4
}

ISR(PCINT2_vect){
	static uint8_t value = 0;

	_delay_ms(20);
	if (!(PIND & (1 << PIND4))) {
		--value;
		refresh(value);
	}
}

ISR(INT0_vect){
	static uint8_t value = 0;

	_delay_ms(20);
	if (!(PIND & (1 << PIND2))) {
		++value;
		refresh(value);
	}
}

int main() {
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	PCICR |= (1 << PCIE2); // Pin chancge interrupt control register 2 to enable interupt on PIN2x
	PCMSK2 |= (1 << PCINT20);

	EICRA |= (1 << ISC01); 
	EIMSK |= (1 << INT0);

	sei();

	while (1);
	
}