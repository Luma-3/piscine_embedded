#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define CLOCK F_CPU / 1024

int on_press(uint8_t PIN) {
	volatile uint8_t i = 0;
	if (!(PIND & (1 << PIN))) {
		_delay_ms(20);
		i++;
	}
	if ((PIND & (1 << PIN))) {
		i++;
	}

	if (i == 2) {
		return 1;
	}
	return 0;
}


int main() {
	DDRB |= (1 << PB1);
	float value = 0.1;

	// COM1A1 for clean OC1A on match

	// active WGM11 & WGM 12 & WGM13 for fast PWM mode (page 109 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
	TCCR1A |=  (1 << COM1A1) | (1 << WGM11);
	TCCR1B |=  (1 << CS12) | (1 << CS10) | (1 << WGM12) | (1 << WGM13);
	
	//OCR1A is limite for down flag (because COM1A1 is active) see (page 108 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf) 
	OCR1A = CLOCK * value;
	// ICR1 is limite of periode (TOP) 
	ICR1 = CLOCK ;


	while (1) {

		if (on_press(PIND2)) {
			if (value < 1) {
				value += 0.1;
			}
		}

		if (on_press(PIND4)) {
			if (value > 0.1) {
				value -= 0.1;
				
			}
		}
		OCR1A = CLOCK * value;
	}
}