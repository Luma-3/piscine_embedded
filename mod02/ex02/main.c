#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

#define BAUD 115200
// Equation for calculate baud rate (page 146 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
#define UBRR (float)F_CPU / (16UL * BAUD) - 1

int main() {
	uart_init((unsigned int)round(UBRR));

	while(1){
		char c = uart_rx();
		uart_tx(c);
	};
}
