#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "util.h"	

void adc_init() {
	ADMUX |= BV(REFS0) | BV(REFS1); // To active AVcc external | without ADLAR no left adjust (10- bit);

	ADCSRA = BV(ADEN) | BV(ADPS2) | BV(ADPS1);

	ADCSRA |= BV(ADEN); // to activate adc !
}

uint16_t adc_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	_delay_ms(5);

	ADCSRA |= BV(ADSC); // Start convertion

	while (ADCSRA & BV(ADSC)); // Buffer is full

	return ADC; // Retrun ADC 10 bit
}

float get_temp() {
	uint16_t adc_val = adc_read(8);

	/**
	 * Formule : Referance array in (p-215)
	 * Slope of function:
	 * 
	 * 		m = (T2 - T1) / (ADC2 - ADC1)
	 * 		  = (25 - (-40)) / 352 - 269
	 * 		  = 65 / 83
	 * 		  = 0.783
	 * 
	 * m = slope
	 * ADC = value
	 * b = offset (x)
	 * 
	 * offset of function: 
	 * 
	 * 		T = m * ADC + b
	 * 	  -40 = 0.783 * 269 + b
	 *	  -40 = 210.6 + b
	 *
	 *		b = -40 - 210.6
	 * 		b = 250.6
	 * 
	 * linear function : ax+b
	 */

    return (0.783 * adc_val - 250.6);
}


int main() {
	uart_init(UBRR);
	adc_init();
	uint16_t value = 0;
	float voltage  = 0;

	while (1) {
		uart_print_num((uint16_t)get_temp()) ;
		uart_printstr("\r\n");
		_delay_ms(500);
	}
}