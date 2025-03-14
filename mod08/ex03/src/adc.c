#include "adc.h"

void adc_init_8b() {
	ADMUX |= BV(REFS0) | BV(ADLAR); // To active AVcc external | without ADLAR no left adjust (10- bit);

	ADCSRA = BV(ADEN) | BV(ADPS2) | BV(ADPS1) | BV(ADIE);

	ADCSRA |= BV(ADEN); // to activate adc !
}

void adc_init_10b() {
	ADMUX |= BV(REFS0); // To active AVcc external | without ADLAR no left adjust (10- bit);

	ADCSRA = BV(ADEN) | BV(ADPS2) | BV(ADPS1) | BV(ADIE);

	ADCSRA |= BV(ADEN); // to activate adc !
}

uint8_t adc_read_8b(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	_delay_ms(5);

	ADCSRA |= BV(ADSC); // Start convertion

	while (ADCSRA & BV(ADSC)); // Buffer is full

	return ADCH; // Retrun ADC 10 bit
}

uint16_t adc_read_10b(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	_delay_ms(5);

	ADCSRA |= BV(ADSC); // Start convertion

	while (ADCSRA & BV(ADSC)); // Buffer is full

	return ADC; // Retrun ADC 10 bit
}