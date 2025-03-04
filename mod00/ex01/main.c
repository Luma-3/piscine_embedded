#include <avr/io.h>

int main(void) {
	// ATmega doc page 59 - 73 https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

	// DDRX is register for set input (0) or output (1)
	// DDRB is data direction register for B

	// DDB0 is specific port 0 for DDB
	DDRB =  (1 << DDB0);

	// PORTX is for on or off (0-1)
	// PORTB is Port register for B

	// PB0 is port 0
	PORTB = (1 << PB0);

	//while for non end programme 
	while (1) {}
}