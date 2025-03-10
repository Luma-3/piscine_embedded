#include "util.h"

#define F_SCL 100000

void i2c_init() {
	TWBR = ((F_CPU / F_SCL) - 16) / 2;
	TWCR |= BV(TWEN);
}

void i2c_start() {
	
}	

int main() {
	_loop();
}