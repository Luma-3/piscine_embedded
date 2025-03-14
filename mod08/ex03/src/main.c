#include "util.h"
#include "adc.h"


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

void sk9822_send_color(uint8_t led_index, uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue) {
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



int main() {
	SPI_init();
	adc_init_8b();
	sk9822_init();
	_loop() {
		uint8_t active_led = (adc_read_8b(0) * NUM_LEDS) / 255;

		for (uint8_t i = 0; i < NUM_LEDS; i++) {
			if (i < active_led) {
				sk9822_send_color(i, 31, 255, 0, 0);  // LEDs actives en rouge
			} else {
				sk9822_send_color(i, 0, 0, 0, 0);  // LEDs éteintes
			}
		}
		sk9822_show();
	}
}