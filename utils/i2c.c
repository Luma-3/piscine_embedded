#include "i2c.h"
#include "util.h"

void i2c_init() {
	TWBR = ((F_CPU / F_SCL) - 16) / 2; // calcule for clock speed find in (p-180)
	TWCR = BV(TWEN); // Enable i2c
}

void i2c_start(uint8_t adress_rw) {
	/**
	 * TWINT (Interupt Flag (ACK-NACK)) 
	 * TWEM Enable i2c
	 * TWSTA for send bit start
	 */
	TWCR  = BV(TWINT) | BV(TWSTA) | BV(TWEN); 
	while (!(TWCR & BV(TWINT))); // Wait TWINT flag is up

	/**
	 *  Send 0x38 (slave adress of  AHT20 (p-12 in https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf))
	 *  bit shift 1 to keep space for READ/WRITE bit
	 */
	TWDR  = adress_rw; 

	/**
	 * TWINT Clear TWINT flag for continue
	 * TWEM Enable i2c
	 */
	TWCR = BV(TWINT) | BV(TWEN);
	while (!(TWCR & BV(TWINT))); // WAIT ACK

}

void i2c_stop() {
	TWCR = BV(TWINT) |  BV(TWEN) | BV(TWSTO); // TWSTO for send STOP condition
	while ((TWCR) & BV(TWSTO)); // Wait STOP bit is send
}

void i2c_write(unsigned char data) {
	TWDR = data; // Transmite data
	TWCR = BV(TWINT) | BV(TWEN); // Clear TWINT flag for continue
	while (!(TWCR & BV(TWINT))); // Wait ACK
}

uint8_t i2c_read_ack() {
	TWCR = BV(TWINT) | BV(TWEN) | BV(TWEA); // TWEA for active ACK
	while (!(TWCR & BV(TWINT)));
	return TWDR;
}

uint8_t i2c_read_nack() {
	TWCR = BV(TWINT) | BV(TWEN); // TWEA for active ACK
	while (!(TWCR & BV(TWINT)));
	return TWDR;
}