
#include <string.h>

#include "uart.h"
#include "eeprom.h"

#define S_SEP 0x1C
#define G_SEP 0x1D
#define C_SEP 0x1F
#define S_VOID 0x00

void write_pair(char *key, char *value, uint16_t i) {
	EEPROM_write(i, S_SEP);
	EEPROM_W_tab(i + 1, (uint8_t *)key, 16);
	// EEPROM_sec_write(i + 17, G_SEP);
	EEPROM_W_tab(i + 17, (uint8_t *)value, 16);
	// EEPROM_sec_write(i + 34, C_SEP);
}

int16_t find_key(char *key) {
	for (uint16_t i = 0; i < 1024; i += 33) {
		if (EEPROM_read(i) != S_SEP) continue;
		uint8_t has_null = 0;
		for (uint16_t j = 0; j < 16; j++) {
			if (!has_null && key[j] == '\0') {
				has_null = 1;
			}
			if (EEPROM_read(i + 1 + j) != ((has_null) ? '\0' : key[j])) break;
			if (j == 15) return (i);
		}
	}
	return (-1);
}

void write_cmd(char *key, char *value) {
	if (find_key(key) >= 0){
		uart_printstr("already exists\r\n");
		return;
	}
	for (uint16_t i = 0; i < 1024; i += 33) {
		if (i + 32 > 1023) break;
		if (EEPROM_read(i) != S_SEP) {
			write_pair(key, value, i);
			uart_printstr("done ! writen at: ");
			uart_print_hex((i >> 8) & 0xFF);
			uart_print_hex(i & 0xFF);
			return;
		}
	}
	uart_printstr("no space left\r\n");
}

void read_cmd(char *key) {
	int16_t address = find_key(key);
	if (address < 0) {
		uart_printstr("empty\r\n");
		return;
	}
	for (uint8_t i = 0; i < 16; i++) {
		uart_tx((char)EEPROM_read(address + 17 + i));
	}
}

void forget_cmd(char *key) {
	int16_t address = find_key(key);
	if (address < 0) {
		uart_printstr("not found\r\n");
		return;
	}
	EEPROM_sec_write(address, 0x00);
}

uint8_t is_space(char c) {
	if (c == ' ' || c == '\n' || c == '\r'|| c == '\t' || c == '\v' || c == '\f');
}

char *get_buffer(char **input) {
	if (**input != '"') return NULL;
	(*input)++;

	char *start = *input;
	while (**input && **input != '"') (*input)++;

	if (**input != '"') return NULL;
	**input = '\0';
	(*input)++;

	return start;
}

void cli(char *input) {

	char *cmd;
	char *save;
	char *key = NULL;
	char *val = NULL;

	while (*input && is_space(*input)) input++;
	cmd = strtok_r(input, " ", &save);

	while (*save && is_space(*save)) save++;
	key = get_buffer(&save);
	save++;

	while (*save && is_space(*save)) save++;
	val = get_buffer(&save);

	if (strlen(key) > 16) {
		uart_printstr("Key too long (16)\r\n");
		return;
	}
	if (strlen(val) > 16) {
		uart_printstr("value is too long (16)\r\n");
		return;
	}

	if (strncmp(cmd, "READ", strlen(cmd)) == 0 && key) {
		read_cmd(key);
	}
	else if (strncmp(cmd, "PRINT", strlen(cmd)) == 0 ) {
		EEPROM_dump();
	}
	else if (strncmp(cmd, "FORGET", strlen(cmd)) == 0 && key) {
		forget_cmd(key);
	}
	else if (strncmp(cmd, "WRITE", strlen(cmd)) == 0 && key && val) {
		write_cmd(key, val);
	}
	else {
		uart_printstr("Command not found !");
	}

	uart_printstr("\r\n");
}