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


#endif // _UTIL_H_