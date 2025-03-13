#include "util.h"
#include "uart.h"

#include "eeprom.h"
#include <string.h>

uint8_t is_hex_char(char c) {
	return (('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'));
}

char ft_toupper(char c) {
	return (c >= 'a' && c <= 'z') ? c - 32 : c;
}

uint8_t hex_to_int(const char hex) {
	char val = ft_toupper(hex);
	return (val >= 'A') ? (val - 'A' + 10) : (val - '0');
}
	

int main() {
	uart_init(UBRR);

	// EEPROM_dump();
	_loop() {
		char address_buff[4];
		get_input(address_buff, 3, "ADDRESS: 0x", &is_hex_char);
		if (strlen(address_buff) != 3) {
			uart_printstr("\r\nInvalide address format\r\n");
			continue;
		}
		uint16_t address = (hex_to_int(address_buff[0]) << 8) | (hex_to_int(address_buff[1]) << 4 ) | hex_to_int(address_buff[2]);
		if (address >= 1024) {
			uart_printstr("\r\nInvalide address (too-high)\r\n");
			continue;
		}


		char value_buff[3];
		get_input(value_buff, 2, "\r\nValue: 0x", &is_hex_char);
		if (strlen(value_buff) != 2) {
			uart_printstr("\r\nInvalide value format\r\n");
			continue;
		}
		uint8_t value = (hex_to_int(value_buff[0]) << 4 ) | hex_to_int(value_buff[1]);

		if (EEPROM_read(address) == value) {
			uart_printstr("\r\nValue is already set at this address\r\n");
			continue;
		}

		EEPROM_write(address, value);
		EEPROM_dump(address);
		uart_printstr("\r\n\r\n");
	}
}