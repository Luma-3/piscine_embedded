#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "util.h"

#define R (1 << PD5)
#define G (1 << PD6)
#define B (1 << PD3)

void adc_init() {
	ADMUX |= BV(REFS0) | BV(ADLAR); // To active AVcc external | without ADLAR no left adjust (10- bit);

	ADCSRA = BV(ADEN) | BV(ADPS2) | BV(ADPS1);

	ADCSRA |= BV(ADEN); // to activate adc !
}

void adc_int(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	ADCSRA |= BV(ADIE);
	ADCSRA |= BV(ADSC); // Start convertion
}

uint8_t adc_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	_delay_ms(5);

	ADCSRA |= BV(ADSC); // Start convertion

	while (ADCSRA & BV(ADSC)); // Buffer is full

	return ADCH; // Retrun ADC 10 bit
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	OCR0A = g;
	OCR0B = r;
	OCR2B = b;
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void init_rgb() {
	DDRD |= R | G | B;
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1); // Fast PWM Clear OCR0A OCR0B
	TCCR0B |= (1 << CS00); // Active  Timer divid 1
	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1); // Fast PWM  Clear OCR2B
	TCCR2B |= (1 << CS20);// Active  Timer divid 1
}

ISR(ADC_vect) {
	uint8_t val = adc_read(0); 
	PORTB = (val >= 64)	 << PB0
		|	(val >= 128) << PB1
		|	(val >= 192) << PB2
		|	(val == 255) << PB4;
	wheel(val);
}


int main() {
	adc_init();
	adc_int(0);
	init_rgb();
	DDRB |=  BV(PB0) | BV(PB1) | BV(PB2) | BV(PB4);
	_SEI();

	while (1);
}