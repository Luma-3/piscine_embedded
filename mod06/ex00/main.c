#include "util.h"
#include "uart.h"

#include "util/twi.h"

#define F_SCL 100000

void i2c_init() {
	TWBR = ((F_CPU / F_SCL) - 16) / 2; // calcule for clock speed find in (p-180)
	TWCR = BV(TWEN); // Enable i2c
}

void i2c_start() {
	/**
	 * TWINT (Interupt Flag (ACK-NACK)) 
	 * TWEM Enable i2c
	 * TWSTA for send bit start
	 */
	TWCR  = BV(TWINT) | BV(TWSTA) | BV(TWEN); 
	while (!(TWCR & BV(TWINT))); // Wait TWINT flag is up
	uart_print_hex(TWSR & 0xF8); // TWSR is register status 0xF8 for 5 bit left mask
	uart_printstr(", ");

	/**
	 *  Send 0x38 (slave adress of  AHT20 (p-12 in https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf))
	 *  bit shift 1 to keep space for READ/WRITE bit
	 */
	TWDR  = (0x38 << 1) | TW_WRITE; 

	/**
	 * TWINT Clear TWINT flag for continue
	 * TWEM Enable i2c
	 */
	TWCR = BV(TWINT) | BV(TWEN);
	while (!(TWCR & BV(TWINT))); // WAIT ACK
	uart_print_hex(TWSR & 0xF8);

}

void i2c_stop() {
	TWCR = BV(TWINT) |  BV(TWEN) | BV(TWSTO); // TWSTO for send STOP condition
	while ((TWCR) & BV(TWSTO)); // Wait STOP bit is send
}

int main() {
	uart_init(UBRR);
	i2c_init();
	_loop() {
		i2c_start();
		i2c_stop();
		uart_printstr("\r\n");
	}
}