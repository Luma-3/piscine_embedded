#include "util.h"
#include "adc.h"
#include "uart.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>


#define SPI_DDR    DDRB
#define SPI_PORT   PORTB
#define MOSI       PB3
#define SCK        PB5
#define SS 			PB2

#define NUM_LEDS 3

uint8_t led_buffer[NUM_LEDS][4];

//SK9822 moz-extension://b0e890ae-612d-42bf-84f0-a794621ae57d/pdfjs/web/viewer.html?file=https%3A%2F%2Fwww.pololu.com%2Ffile%2F0J1234%2Fsk9822_datasheet.pdf

//SPI Serial Port Interface
void SPI_init() {
	SPI_DDR |= BV(MOSI) | BV(SCK) | BV(SS); // MOSI Master data output; SCK master clock output; SS to output mod for avoid slave mod
	SPCR = BV(SPE) | BV(MSTR) | BV(SPR0); // SPE : SPI Enable; MSTR for master mode; SPR0 for Frenquency/16 1Mhz good for LED SK9822
}

void SPI_send(uint8_t data) {
	SPDR = data; // DATA register
	while (!(SPSR & (1 << SPIF)));
}

void sk9822_set_color(uint8_t led_index, uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue) {
    if (led_index >= NUM_LEDS) return;
    led_buffer[led_index][0] = 0b11100000 | (brightness & 0x1F);
    led_buffer[led_index][1] = blue;
    led_buffer[led_index][2] = green;
    led_buffer[led_index][3] = red;
}

void sk9822_show() {
    SPI_send(0x00); SPI_send(0x00); SPI_send(0x00); SPI_send(0x00);  // Start Frame
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        SPI_send(led_buffer[i][0]);  // BRIGHTNESS
        SPI_send(led_buffer[i][1]);  // BLUE
        SPI_send(led_buffer[i][2]);  // GREEN
        SPI_send(led_buffer[i][3]);  // RED
    }
    for (uint8_t i = 0; i < (NUM_LEDS + 1) / 2; i++) {
        SPI_send(0xFF);  // End Frame
    }
}

void sk9822_init() {
	for (uint8_t i = 0; i < NUM_LEDS; i++) {
        led_buffer[i][0] = 0b11100000 | 0; // BRIGHTNESS
        led_buffer[i][1] = 0x00;  // BLUE
        led_buffer[i][2] = 0x00;  // GREEN
        led_buffer[i][3] = 0x00;  // RED
    }
}


void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		sk9822_set_color(0, 31, 255 - pos * 3, 0, pos * 3);
		sk9822_set_color(1, 31, 255 - pos * 3, 0, pos * 3);
		sk9822_set_color(2, 31, 255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		sk9822_set_color(0, 31, 0, pos * 3, 255 - pos * 3);
		sk9822_set_color(1, 31, 0, pos * 3, 255 - pos * 3);
		sk9822_set_color(2, 31, 0, pos * 3, 255 - pos * 3);
	} else {
		pos = pos - 170;
		sk9822_set_color(0, 31, pos * 3, 255 - pos * 3, 0);
		sk9822_set_color(1, 31, pos * 3, 255 - pos * 3, 0);
		sk9822_set_color(2, 31, pos * 3, 255 - pos * 3, 0);
	}
}


uint8_t is_hex(char c) {
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || c == 'D');
}

uint8_t constraint(char c) {
	return (c >= 32 && c <= 126);
}

void hex_to_rgb(const char* hex, uint8_t* red, uint8_t* green, uint8_t* blue) {
    sscanf(hex, "%2hhx%2hhx%2hhx", red, green, blue);
}

void parse_input(const char *input) {

	if (strlen(input) == 11 && strncmp(input, "FULLRAINBOW", 11) == 0){
		TCCR0B |= BV(CS00) | BV(CS02) ;
		return;
	} 
	if (strlen(input) != 8) {
		uart_printstr("format Error\r\n");
		return;
	}
	uint8_t i = 0;
	while(input[i]) {
		if (!is_hex(input[i])) {
			uart_printstr("format Error\r\n");
			return;
		}
		i++;
	}

	TCCR0B &= ~(BV(CS00) | BV(CS02));

	char hex_color[7];
	char led_id_str[3];
	uint8_t led_id;

	strncpy(hex_color, input, 6);
	hex_color[6] = '\0'; 

	strncpy(led_id_str, input + 6, 2);
    led_id_str[2] = '\0';

	if (led_id_str[0] != 'D') {
		uart_printstr("format Error\r\n");
		return;
	}
	led_id = strtol(led_id_str + 1, NULL, 10);
	led_id -= 6;

	uint8_t red, green, blue;
	hex_to_rgb(hex_color, &red, &green, &blue);

	if (led_id >= 0 && led_id <= 2) {
		sk9822_set_color(led_id, 31, red, green, blue);
	}
	else {
		uart_printstr("Wrong led id\r\n");
	}
}

uint8_t wheel_val = 0;

ISR(TIMER0_OVF_vect) {
	wheel_val++;
	wheel(wheel_val);
	sk9822_show();
}

void init_timer() {
	TCCR0A |= (1 << WGM00) | (1 << WGM01); // Fast PWM Clear OCR0A
	TIMSK0 |= BV(TOIE0);
}

int main() {

	uart_init(UBRR);
	SPI_init();
	sk9822_init();
	sk9822_show();
	init_timer();
	_SEI()
	_loop() {
		char buff[12] = "";
		get_input(buff, 11, "LED SPI #", &constraint);
		uart_printstr("\r\n");
		parse_input(buff);
		sk9822_show();
	}
}