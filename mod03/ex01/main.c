#include <avr/io.h>
#include <util/delay.h>

#define R (1 << PD5)
#define G (1 << PD6)
#define B (1 << PD3)

#define RGB_LED(A) {			\
	PORTD ^= A;					\
	_delay_ms(1000);			\
	PORTD ^= A;					\
}

int main() {
	// LED OUTPUT/INPUT
	DDRD |= R | G | B;
	while (1) {
		RGB_LED(R);
		RGB_LED(G);
		RGB_LED(B);
		RGB_LED(R | G);
		RGB_LED(G | B);
		RGB_LED(R | B);
		RGB_LED(R | G | B);
	}
}
