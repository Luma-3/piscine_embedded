#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "util.h"	

void adc_init() {
	ADMUX |= BV(REFS0); // To active AVcc external | without ADLAR no left adjust (10- bit);

	ADCSRA = BV(ADEN) | BV(ADPS2) | BV(ADPS1);

	ADCSRA |= BV(ADEN); // to activate adc !
}

uint16_t adc_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

	ADCSRA |= BV(ADSC); // Start convertion

	while (ADCSRA & BV(ADSC)); // Buffer is full

	return ADC; // Retrun ADC 10 bit
}

int main() {
	uart_init(UBRR);
	adc_init();
	uint16_t value = 0;

	while (1) {
		value = adc_read(0);
		uart_print_num(value);
		uart_printstr(", ");
		value = adc_read(1);
		uart_print_num(value);
		uart_printstr(", ");
		value = adc_read(2);
		uart_print_num(value);
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}