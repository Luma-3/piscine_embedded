#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "util.h"	

void adc_init() {
	ADMUX |= BV(REFS0) |  BV(ADLAR); // To active AVcc external | ADLAR for left adjust (8 - bit);

	ADCSRA = BV(ADEN) | BV(ADPS2) | BV(ADPS1); // Prescaler (ADPS2 | ADPS1) for 64 prescale (precise convertion echantillonage)

	ADCSRA |= BV(ADEN); // to activate adc !
}

uint8_t adc_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

	ADCSRA |= BV(ADSC); // Start convertion

	while (ADCSRA & BV(ADSC)); // Buffer is full

	return ADCH; // Retrun Left part of ADC
}

int main() {
	uart_init(UBRR);
	adc_init();

	while (1) {
		uint8_t value = adc_read(0);
		uart_print_hex(value);
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}