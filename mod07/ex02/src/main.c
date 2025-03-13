#include "util.h"
#include "uart.h"

#include "eeprom.h"
#include "cli.h"

char ft_toupper(char c) {
	return (c >= 'a' && c <= 'z') ? c - 32 : c;
}

uint8_t is_print(char c) {
	return (c >= 32 && c <= 126 );
}

int main() {
	uart_init(UBRR);
	_loop() {
		char cmd_buff[255] = "";
		get_input(cmd_buff, 254, "EEPROM> ", &is_print);
		uart_printstr("\r\n");
		cli(cmd_buff);
	}
}