#include "util.h"
#include "uart.h"

#define SPI_DDR    DDRB
#define SPI_PORT   PORTB
#define MOSI       PB3
#define SCK        PB5
#define SS 			PB2

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

void start_frame() {
	SPI_send(0x00);
	SPI_send(0x00);
	SPI_send(0x00);
	SPI_send(0x00);
}

void end_frame(uint8_t num_leds) {
    for (uint8_t i = 0; i < (num_leds + 1) / 2; i++) {
        SPI_send(0xFF);
    }
}

void sk9822_send_color(uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue) {
	start_frame();

	SPI_send(0b11100000 | (brightness & 0x1F));
    SPI_send(blue);
    SPI_send(green);
    SPI_send(red);

	end_frame(1);
}

int rgb() {
	sk9822_send_color(31, 255, 0, 0);
	_delay_ms(1000);
	sk9822_send_color(31, 0, 255, 0);
	_delay_ms(1000);
	sk9822_send_color(31, 0, 0, 255);
	_delay_ms(1000);
	sk9822_send_color(31, 255, 255, 0);
	_delay_ms(1000);
	sk9822_send_color(31, 0, 255, 255);
	_delay_ms(1000);
	sk9822_send_color(31, 255, 0, 255);
	_delay_ms(1000);
	sk9822_send_color(31, 255, 255, 255);
	_delay_ms(1000);
}


int main() {

	SPI_init();
	_loop() {
		rgb();
	}
}