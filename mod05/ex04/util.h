#ifndef _UTIL_H_
#define _UTIL_H_

#include <avr/io.h>
#include <util/delay.h>


#ifndef F_CPU
#	define F_CPU 16000000UL
#endif

#ifndef BAUD
#	define BAUD 115200
#endif

#define BV(bit) (1 << (bit))

#define _SEI() {SREG |= BV(SREG_I);}


#endif // _UTIL_H_