#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

void adc_init() {
	ADMUX |= (1 << REFS0); // To active AVcc external | ADLAR for left adjust (8 - bit);

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);

	ADCSRA |= (1 << ADEN); // to activate adc !
}

uint8_t adc_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

	ADCSRA |= (1 << ADSC); // Start convertion

	while (ADCSRA & (1 << ADSC));

	return ADC;
}

int main() {
	uart_init((unsigned int)round(UBRR));
	adc_init();

	
	while (1) {
		uint8_t value = adc_read(0);
		uart_print_hex(value);
		uart_printstr("\r\n");
		_delay_ms(500);

	}
}