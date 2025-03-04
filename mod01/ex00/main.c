#include <avr/io.h>

int main() {
	DDRB |= (1 << PB1);
	PORTB &= ~(1 << PB1);

	while (1) {
		// 16000000 * 0.5 / 4 = 2000000
		//	^         ^     ^
		//	|		  |     numbre of instruction for a while loop process (in ASM)
		//	|		  nb of ms
		//	frequency CPU Clock 16Mhz
		uint32_t i = 2000000;
		while (i--);

		PORTB ^= (1 << PB1);
	}
}