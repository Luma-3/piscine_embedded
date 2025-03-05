#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

#define BAUD 115200
// Equation for calculate baud rate (page 146 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
#define UBRR (float)F_CPU / (16UL * BAUD) - 1

ISR(USART_RX_vect) {
	char c = UDR0;
	if ('A' <= c && c <= 'Z') {
		c += 32;
	}
	else if ('a' <= c && c <= 'z') {
		c -= 32;
	}
	uart_tx(c);
}

int main() {
	uart_init((unsigned int)round(UBRR));
	uart_rx_interupt_enable();
	sei();

	while(1);
}
