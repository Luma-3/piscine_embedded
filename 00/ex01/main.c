#include <avr/io.h>

int main(void) {
	// ATmega doc page 59 - 73

	// DD is for set input or output utility
	// DDRB is data direction register for B
	// DDB0 is specific port 0 for DDB
	DDRB =  (1 << DDB0);

	// PORT is for on or off ine spec port
	// PORTB is Port register for B
	// PB0 is port 0
	PORTB = (1 << PB0);

	//while for non end programme 
	while (1) {}
	return 0;
}