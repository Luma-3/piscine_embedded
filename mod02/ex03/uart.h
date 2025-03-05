#ifndef _UART_H
#define _UART_H



void uart_init(unsigned int ubrr);
void uart_tx(char c);
char uart_rx(void);

void uart_printstr(const char *str);

void uart_rx_interupt_enable();

#endif // _UART_H

