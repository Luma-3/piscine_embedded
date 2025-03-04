#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define CLOCK F_CPU / 1024


int main() {
	DDRB |= (1 << PB1);

	// COM1A1 for clean OC1A on match

	// active WGM11 & WGM 12 & WGM13 for fast PWM mode (page 109 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
	TCCR1A |=  (1 << COM1A1) | (1 << WGM11);
	TCCR1B |=  (1 << CS12) | (1 << CS10) | (1 << WGM12) | (1 << WGM13);
	
	//OCR1A is limite for down flag (because COM1A1 is active) see (page 108 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf) 
	OCR1A = CLOCK * 0.1;
	// ICR1 is limite of periode (TOP) 
	ICR1 = CLOCK ;


	while (1) {}
}