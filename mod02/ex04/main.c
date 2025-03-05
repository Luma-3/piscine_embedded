#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

#define BAUD 115200
// Equation for calculate baud rate (page 146 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
#define UBRR (float)F_CPU / (16UL * BAUD) - 1

ISR(USART_RX_vect) {
}

char get_char_echo() {
	char c = uart_rx();
	uart_tx(c);
	return (c);
}

char get_input(const char *label) {
	uart_printstr(label);
	while (1) {
		
	}
}

int main() {
	uart_init((unsigned int)round(UBRR));
	uart_rx_interupt_enable();
	sei();

	const char *pass = "pass";
	const char *uname = "admin";

	while(1) {
		uart_printstr("Enter your login:");

	}
}
