#include <avr/io.h>


int main() {
	DDRB |= (1 << PB1);

	// CS12 & CS10 for 1024 devide frequency
	// WGM12 for mode CTC (OCR1A is TOP) (page 109 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
	TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);
	TCCR1A = (1 << COM1A0);

	OCR1A = 7812; // 0.5s 16Mhz / 1024 / 2

	while (1) {}
}