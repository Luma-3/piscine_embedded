#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <avr/eeprom.h>

#include "util.h"

#define RED "\033[31m"
#define RESET "\033[0m"
#define GRAY "\033[90m"

uint8_t EEPROM_read(uint16_t address);

void EEPROM_write(uint16_t address, uint8_t value);

void EEPROM_dump(uint16_t highlight_address);

#endif // _EEPROM_H_