#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

#define BAUD 115200
// Equation for calculate baud rate (page 146 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
#define UBRR (float)F_CPU / (16UL * BAUD) - 1

char get_char(uint8_t echo, uint8_t crypt) {
	char c = uart_rx();
	if (echo) {
		if (c >= 32 && c <= 126)
			uart_tx(crypt ? '*' : c);
	}
	return (c);
}

uint8_t is_printable(char c) {
	return ((c >= 32 && c <= 126) || c == '\b' || c == 127 || c == '\r');
}


void get_input(const char *label,  char *buffer, uint8_t crypt) {
	char c;
	uint8_t i = 0;

	uart_printstr(label);
	while (1) {
		c = get_char((i == 255 ? 0 : 1), crypt);
		if (!is_printable(c)) continue;
		if (c == '\r') {
			uart_printstr("\n\r");
			buffer[i] = '\0';
			break;
		}
		else if ((c == '\b' || c == 127)) {
			if (i > 0) {
				uart_printstr("\b \b");
				i--;
			}
			continue;
		}
		else if (i < 255) {
			buffer[i] = c;
			i++;
		}
	}
}

uint8_t ft_strcmp(const char *a, const char *b) {
	while (*b) {
		if (*a++ != *b++) {
			return (*--b - *--a);
		}
	}
	return (0);
}

void success() {
	uart_printstr("Shall we play a game?\n\r");
	for (uint8_t i = 0; i < 3; i++) {
		PORTB ^= (1 << PB0);
		_delay_ms(200);
		PORTB ^= (1 << PB1);
		_delay_ms(200);
		PORTB ^= (1 << PB2);
		_delay_ms(200);
		PORTB ^= (1 << PB4);
		_delay_ms(600);
		PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
	}
	while (1) {
		PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
		_delay_ms(300);
		PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
		_delay_ms(300);
	}
}

void error() {
	uart_printstr("Bad combinaison username/password\n\r");
	for (uint8_t i = 0; i < 3; i++) {
		PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
		_delay_ms(400);
		PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
		_delay_ms(200);
	}
}

int main() {
	uart_init((unsigned int)round(UBRR));

	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

	const char *pass = "pass";
	const char *uname = "admin";
	char buff_pass[255] = "";
	char buff_uname[255] = "";

	while(1) {
		uart_printstr("Enter your login:\n\r");
		get_input("\tUsername: ", buff_uname, 0);
		get_input("\tPassword: ", buff_pass, 1);
		if (ft_strcmp(buff_uname, uname) == 0 && ft_strcmp(buff_pass, pass) == 0){
			success();
		}
		else {
			error();
		}
	}
}
