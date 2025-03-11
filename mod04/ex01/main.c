#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "timer.h"

volatile uint8_t duty_cycle = 0;
volatile int8_t step = 1;

#define _SEI() {SREG |= (1<< SREG_I);}


ISR(TIMER0_COMPA_vect) {
	static uint8_t counter = 0;

	counter++;
	if (counter >= 2) { // 2 for 2 cycle of 1 ms 255 * 2 510 ms for up to down or down to up
		counter = 0;
		duty_cycle += step;

		if (duty_cycle == 255 || duty_cycle == 0) {
			step = -step; // Invert dir
		}
		TIM1_SET_OCR1A(duty_cycle);
	}
}

int main() {
	DDRB |= (1 << PB1); // output led
	TIM0_CTC_MODE();
	TIM0_SET_PRESCALER_64(); // 1 tick 4 µs
	TIM0_SET_OCR0A(249); // 250 * 4 = 1000 µs == 1ms (-1 for 0)
	TIM0_ENABLE_INTERRUPT_COMPARE_MATCH();

	TIM1_FAST_PWM_8BIT_MODE();
	TIM1_SET_PRESCALER_8();
	TCCR1A |= (1 << COM1A1);

	TIM1_SET_OCR1A(duty_cycle);

	_SEI();
	while (1);
}