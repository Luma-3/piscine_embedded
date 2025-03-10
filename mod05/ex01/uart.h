#ifndef _UART_H
#define _UART_H


#include "util.h"

// Equation for calculate baud rate (page 146 in https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
#define UBRR (float)F_CPU / (16UL * BAUD)

void uart_init(unsigned int ubrr);
void uart_tx(char c);
char uart_rx(void);

void uart_printstr(const char *str);

void uart_rx_interupt_enable();

void uart_print_hex(uint8_t value);

#endif // _UART_H

