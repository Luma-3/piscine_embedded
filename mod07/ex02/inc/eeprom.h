#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <avr/eeprom.h>

#include "util.h"

#define RED "\033[31m"
#define RESET "\033[0m"
#define GRAY "\033[90m"

uint8_t EEPROM_read(uint16_t address);

void EEPROM_write(uint16_t address, uint8_t value);

void EEPROM_sec_write(uint16_t address, uint8_t data);

void EEPROM_W_tab(uint16_t address, uint8_t *data, uint16_t size);

void EEPROM_dump();

#endif // _EEPROM_H_