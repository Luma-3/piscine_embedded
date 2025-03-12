#include "util.h"
#include "uart.h"


#include <avr/eeprom.h>

uint8_t EEPROM_read(uint16_t address) {
	while(EECR & BV(EEPE)); // Wait EEPROM is ready for operation
	EEAR = address & 0x1FF; // EEAR is EEPROM Address register (7 high bit is reserved.)
	EECR |= EERE; // Active Read
	return EEDR; // EEDR is EEPROM DATA Register (data Read-Write)
}

void EEPROM_dump() {
	char ascii[17];

	for (uint16_t i = 0; i < 1024; i++) {
		// print address by 16
		if (i % 16 == 0) {
			uart_printstr("\r\n");
			uart_print_hex((i >> 8) & 0xFF);
			uart_print_hex(i & 0xFF);
			uart_printstr("\t");
		}

		uint8_t val = EEPROM_read(i);
		uart_print_hex(val);
		uart_tx(' ');

		ascii[i % 16] = (val >= 32 && val <= 126) ? val : '.';
		if (i % 16 == 15) {
			ascii[16] = '\0';
			uart_printstr(" |");
			uart_printstr(ascii);
			uart_tx('|');
		} 
	}
}

int main() {
	uart_init(UBRR);

	EEPROM_dump();
	_loop();
}