#include <avr/io.h>
#include <util/delay.h>

#define RED PD5
#define BLUE PD3
#define GREEN PD6

int main() {
	DDRD |= (1 << RED) | (1<<GREEN)| (1 <<BLUE);
	while (1) {
		PORTD |=(1 << RED);
		_delay_ms(1000);
		PORTD &= ~(1 << RED);
		PORTD |=(1 << GREEN);
		_delay_ms(1000);
		PORTD &= ~(1 << GREEN);
		PORTD |=(1 << BLUE);
		_delay_ms(1000);
		PORTD &= ~(1 << BLUE);
	}
}
