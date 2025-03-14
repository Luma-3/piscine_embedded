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

void uart_print_hex(uint8_t value) {
	char hex[] = "0123456789abcdef";
	uart_tx(hex[(value >> 4) & 0x0F]);
	uart_tx(hex[value & 0x0F]);
}

void uart_print_num(uint16_t value) {
	if (value >= 10) {
		uart_print_num(value / 10);
	}
	uart_tx('0' + (value % 10));
}


////////////////////////////////////////////

uint8_t is_spectial(char c) {
	return (c == '\b' || c == 127 || c == '\r');
}


char get_char(uint8_t echo, uint8_t (*constraint)(char c)) {
	char c = uart_rx();
	if (echo && (constraint(c) || is_spectial(c))) {
		uart_tx(c);
	}
	return (c);
}

void get_input(char *buffer, int buffer_size, const char *label, uint8_t (*constraint)(char c)) {
	char c;
	uint16_t i = 0;

	uart_printstr(label);
	while(1) {
		c = get_char((i == buffer_size) ? 0 : 1, constraint);
		if (!(constraint(c) || is_spectial(c))) continue;
		if (c == '\r') {
			buffer[i] = '\0';
			break;
		}
		else if (c == '\b' || c == 127) {
			if (i > 0) {
				uart_printstr("\b \b");
				--i;
			}
			continue;
		}
		else if (i < buffer_size) {
			buffer[i] = c;
			++i;
		}
	}
}