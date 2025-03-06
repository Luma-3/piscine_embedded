#include <avr/io.h>

#include "uart.h"

void uart_init(unsigned int ubrr) {
	/**
	 * UBRR0 is register of 2 octet for set baud rate of USART
	 */
	UBRR0H |= (unsigned char)(ubrr >> 8);
	UBRR0L |= (unsigned char)ubrr;

	/**
	 *  RXEN0 for activate recieve mode.
	 * 	TXEN0 for activate Transmite mode.
	 * 
	 * 	Register UCSR0B is Control and status of USART segment B
	 * 
	 * @details (page 160 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
	 */ 
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

	/**
	 * UCSZ00 is start of 3 bit part of register for set how many bits are use for data
	 */
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void uart_rx_interupt_enable() {
	// Active Interupt for Rx 
	UCSR0B |= (1 << RXCIE0);
}

void uart_tx(char c) {
	// wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));
	//put data into buffer
	UDR0 = c;
}

char uart_rx(void) {
	// wait for all transmited data
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void uart_printstr(const char *str) {
	while(*str) {
		uart_tx(*str++);
	}
}