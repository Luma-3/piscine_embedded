#ifndef _UART_H
#define _UART_H



void uart_init(unsigned int ubrr);
void uart_tx(char c);
void uart_printstr(const char *str);


#endif // _UART_H

