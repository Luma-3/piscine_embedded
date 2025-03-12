#include "eeprom.h"

#include "uart.h"

uint8_t EEPROM_read(uint16_t address) {
	while(EECR & BV(EEPE)); // Wait EEPROM is ready for operation
	while (SPMCSR & (BV(SPMCSR)));
	EEAR = address; // EEAR is EEPROM Address register (7 high bit is reserved.)
	EECR |= BV(EERE); // Active Read
	return EEDR; // EEDR is EEPROM DATA Register (data Read-Write)
}

void EEPROM_write(uint16_t address, uint8_t data){
	while(EECR & BV(EEPE));
	while (SPMCSR & (BV(SPMCSR)));
	uint8_t oldSERG = SREG; // Save SERG Register
	_CLI();	// Deactivate global interupt for prevent interupt during wrtie into eeprom
	EEAR = address;
	EEDR = data;

	EECR |= BV(EEMPE);
	EECR |= BV(EEPE);

	SREG = oldSERG;
}


void EEPROM_dump(uint16_t highlight_address) {
	char ascii[17];

	for (uint16_t i = 0; i < 1024; i++) {
		// print address by 16
		if (i % 16 == 0) {
			uart_printstr("\r\n");
			uart_print_hex((i >> 8) & 0xFF);
			uart_print_hex(i & 0xFF);
			uart_printstr("\t");
		}

		if (i % 8 == 0 )  uart_printstr("  ");
		
		uint8_t val = EEPROM_read(i);
		switch (val) {
			case 0x00: 
				uart_printstr(GRAY);
				break;
			case 0xff: 
			uart_printstr(GRAY);
				break;
		}

		if (i == highlight_address) uart_printstr(RED);
		uart_print_hex(val);
		uart_printstr(RESET);
		uart_tx(' ');

		ascii[i % 16] = (val >= 32 && val <= 126) ? val : '.';
		if (i % 16 == 15) {
			ascii[16] = '\0';
			uart_printstr("  |");
			uart_printstr(ascii);
			uart_tx('|');
		} 
	}
}