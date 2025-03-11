#include "util.h"
#include "uart.h"

#include "util/twi.h"

#define F_SCL 100000

void i2c_init() {
	TWBR = ((F_CPU / F_SCL) - 16) / 2;
	TWCR |= BV(TWEN);
}

void i2c_start() {
	TWCR  = BV(TWINT) | BV(TWSTA) | BV(TWEN);
	while (!(TWCR & BV(TWINT)));
	uart_print_hex(TWSR & 0xF8);
	uart_printstr(", ");

	TWDR  = (0x38 << 1) | TW_WRITE;
	TWCR = BV(TWINT) | BV(TWEN);
	while (!(TWCR & BV(TWINT)));
	uart_print_hex(TWSR & 0xF8);
}

void i2c_write(unsigned char data) {
	TWDR = data;
	TWCR = BV(TWINT) | BV(TWEN);
	while (!(TWCR & BV(TWINT)));
}

void i2c_stop() {
	TWCR = BV(TWINT) |  BV(TWEN) | BV(TWSTO);
}

int main() {
	uart_init(UBRR);
	i2c_init();

	_delay_ms(40);

	_loop() {
		i2c_start();
		i2c_write(0xAC);

		i2c_stop();
		uart_printstr("\r\n");
	}
}