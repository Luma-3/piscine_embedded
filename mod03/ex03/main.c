#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"

#define BAUD 115200
// Equation for calculate baud rate (page 146 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
#define UBRR (float)F_CPU / (16UL * BAUD) - 1

#define R (1 << PD5)
#define G (1 << PD6)
#define B (1 << PD3)

uint8_t is_hex_char(char c) {
	return (('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F') || c == '\b' || c == 127 || c == '\r');
}

char get_char(uint8_t echo) {
	char c = uart_rx();
	if (echo && is_hex_char(c)) {
		uart_tx(c);
	}
	return (c);
}

void get_hex(char *buffer) {
	char c;
	uint8_t i = 0;

	uart_printstr("Enter Hex color code\n\rColor: #");
	while(1) {
		c = get_char((i == 6) ? 0 : 1);
		if (!is_hex_char(c)) continue;
		if (c == '\r' && i == 6) {
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
		else if (i < 6) {
			buffer[i] = c;
			++i;
		}
	}
	while (*buffer){
		if (*buffer >= 'A' && *buffer <= 'F'){
			*buffer += 32;
		}
		*buffer++;
	}
}

uint8_t hex_to_uint8(const char *hex) {
	uint8_t high =	(hex[0] >= '0' && hex[0] <= '9') ? hex[0] - '0' : hex[0] - 'a' + 10;
	uint8_t low =	(hex[1] >= '0' && hex[1] <= '9') ? hex[1] - '0' : hex[1] - 'a' + 10; 

	return (high << 4) | low;
}

void init_rgb() {
	DDRD |= R | G | B;
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1); // Fast PWM Clear OCR0A OCR0B
	TCCR0B |= (1 << CS00); // Active  Timer divid 1
	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1); // Fast PWM  Clear OCR2B
	TCCR2B |= (1 << CS20);// Active  Timer divid 1
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	OCR0A = g;
	OCR0B = r;
	OCR2B = b;
}

int main() {
	uart_init((unsigned int)round(UBRR));
	init_rgb();

	while (1) {
		char buff[6] = "";

		get_hex(buff);
		set_rgb(hex_to_uint8(buff), hex_to_uint8(buff + 2), hex_to_uint8(buff + 4));
	}
}