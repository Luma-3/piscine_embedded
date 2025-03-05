#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

#define BAUD 115200
// Equation for calculate baud rate (page 146 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
#define UBRR (float)F_CPU / (16UL * BAUD) - 1

ISR(TIMER1_COMPA_vect) {
	uart_printstr("Hello World!\n\r");
}

int main() {
	uart_init((unsigned int)round(UBRR));


	TCCR1B |= (1 << CS10) | (1 << CS12) | (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 31250;

	sei();

	while(1);
}
